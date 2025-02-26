/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jazo <sel-jazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:44:08 by sel-jazo          #+#    #+#             */
/*   Updated: 2025/02/25 16:10:19 by sel-jazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

int	ft_byte_num(unsigned char u, int limit)
{
	if (limit != 0)
		return (limit);
	if (u >= (240))
		return (4);
	if (u >= (224))
		return (3);
	if (u >= (192))
		return (2);
	return (1);
}

void	ft_print(int *a, int *b, char arr[])
{
	int	j;

	j = 0;
	if (*a != *b)
		return ;
	while (j < *b)
	{
		write(1, &arr[j], 1);
		j++;
	}
	*a = 0;
	*b = 0;
}

void	handler(int signum, siginfo_t *info, void *context)
{
	static char				arr[4];
	static int				j;
	static int				limit;
	static int				g_bits;
	static int				g_old_pid;

	(void)context;
	if (info->si_pid != g_old_pid)
	{
		g_bits = 0;
		j = 0;
		limit = 0;
		arr[j] = 0;
	}
	arr[j] = ((arr[j] << 1) | (signum == SIGUSR1));
	if (++g_bits == 8)
	{
		limit = ft_byte_num(arr[j], limit);
		j++;
		ft_print(&j, &limit, arr);
		g_bits = 0;
		arr[j] = 0;
	}
	g_old_pid = info->si_pid;
	kill(info->si_pid, SIGUSR1);
}

void	ft_print_arr(int i, char pid[])
{
	while (i > 0)
	{
		i--;
		write(1, &pid[i], 1);
	}
}

int	main(void)
{
	struct sigaction	sa;
	char				pid[10];
	int					i;
	int					pid_num;

	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO | SA_NODEFER;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	write(1, "PID: ", 5);
	i = 0;
	pid_num = getpid();
	while (pid_num)
	{
		pid[i] = pid_num % 10 + '0';
		pid_num /= 10;
		i++;
	}
	ft_print_arr(i, pid);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
