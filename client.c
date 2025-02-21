#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

volatile sig_atomic_t g_ack_received = 0;
volatile sig_atomic_t g_timeout_counter = 0;

void received_ack(int sig)
{
    if (sig == SIGUSR1)
        g_ack_received = 1;
}

void timeout_handler(int sig)
{
    (void)sig;
    g_timeout_counter++;
}

void setup_timeout(void)
{
    struct sigaction sa;
    
    sa.sa_handler = timeout_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        write(2, "Error setting up timeout handler\n", 31);
        exit(1);
    }
}

void ft_sent(int pid, char c)
{
    int i;
    int max_retries = 3;
    int retry_count;
    
    sigset_t mask, orig_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGALRM);
    
    i = 128;
    while (i > 0)
    {
        retry_count = 0;
        while (retry_count < max_retries)
        {
            g_ack_received = 0;
            g_timeout_counter = 0;
            
            if (kill(pid, (c & i) ? SIGUSR1 : SIGUSR2) == -1)
            {
                write(2, "Error sending signal\n", 20);
                exit(1);
            }
            
            // Block signals to avoid race condition before checking flags
            sigprocmask(SIG_BLOCK, &mask, &orig_mask);
            
            alarm(1);
            while (!g_ack_received && g_timeout_counter == 0)
            {
                sigsuspend(&orig_mask); // Wait for signal
            }
            alarm(0);
            
            sigprocmask(SIG_SETMASK, &orig_mask, NULL);
            
            if (g_ack_received)
                break;
            
            retry_count++;
            if (retry_count == max_retries)
            {
                write(2, "Max retries reached, communication failed\n", 40);
                exit(1);
            }
        }
        
        i >>= 1;
    }
}

int ft_atoi(const char *str)
{
    int result;
    int sign;
    
    while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
        str++;
    
    sign = 1;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    
    result = 0;
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return (result * sign);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        write(2, "Usage: ./client [server_pid] [message]\n", 38);
        return 1;
    }
    
    int pid = ft_atoi(argv[1]);
    if (pid <= 0)
    {
        write(2, "Invalid PID\n", 12);
        return 1;
    }
    
    // Setup signal handlers
    struct sigaction sa;
    sa.sa_handler = received_ack;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        write(2, "Error setting up signal handler\n", 30);
        return 1;
    }
    
    setup_timeout();
    
    // Send message
    int i = 0;
    while (argv[2][i])
        ft_sent(pid, argv[2][i++]);
    ft_sent(pid, '\0');  // Send null terminator
    
    return 0;
}