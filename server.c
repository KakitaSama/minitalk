#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

volatile sig_atomic_t g_sender_pid = 0;
volatile sig_atomic_t g_bit_received = 0;

void ft_receive_sigaction(int signal, siginfo_t *info, void *context)
{
    (void)context;
    g_sender_pid = info->si_pid;
    g_bit_received = 1;
    
    if (signal == SIGUSR1)
        g_bit_received = 2;  // SIGUSR1 = 1 bit
    else if (signal == SIGUSR2)
        g_bit_received = 1;  // SIGUSR2 = 0 bit
}

void handle_character(unsigned char c)
{
    if (c == '\0')
    {
        if (write(1, "\n", 1) == -1)
        {
            write(2, "Write error\n", 12);
            exit(1);
        }
    }
    else
    {
        if (write(1, &c, 1) == -1)
        {
            write(2, "Write error\n", 12);
            exit(1);
        }
    }
}

void ft_putnbr(int n)
{
    char    buffer[12];
    int     i;
    
    if (n == 0)
    {
        write(1, "0\n", 2);
        return;
    }
    
    i = 0;
    while (n > 0)
    {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }
    
    while (--i >= 0)
        write(1, &buffer[i], 1);
    write(1, "\n", 1);
}

void setup_signals(void)
{
    struct sigaction sa;
    
    sa.sa_sigaction = ft_receive_sigaction;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1 ||
        sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        write(2, "Error setting up signal handlers\n", 31);
        exit(1);
    }
}

int main(void)
{
    unsigned char c;
    int bit_count;
    
    setup_signals();
    ft_putnbr(getpid());
    
    while (1)
    {
        c = 0;
        bit_count = 0;
        
        while (bit_count < 8)
        {
            g_bit_received = 0;
            pause();  // Wait for signal
            
            if (g_bit_received)
            {
                c = (c << 1) | (g_bit_received == 2 ? 1 : 0);
                bit_count++;
                
                // Send acknowledgment
                if (kill(g_sender_pid, SIGUSR1) == -1)
                {
                    write(2, "Error sending acknowledgment\n", 27);
                    exit(1);
                }
            }
        }
        
        handle_character(c);
    }
    
    return 0;
}