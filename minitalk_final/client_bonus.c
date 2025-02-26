/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jazo <sel-jazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:29:32 by sel-jazo          #+#    #+#             */
/*   Updated: 2025/02/26 02:28:01 by sel-jazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

static int	g_ack;

void	handler(int signum)
{
	(void)signum;
	g_ack = 1;
}

void	send_char(int pid, char c)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		g_ack = 0;
		if ((c >> bit) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (!g_ack)
			;
		bit--;
	}
}

int	ft_atoi(char *str)
{
	int	num;


	num = 0;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + (*str - '0');
		str++;
	}
	return (num);
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	int					pid;
	char				*str;

	if (argc != 3)
		return (1);
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	pid = ft_atoi(argv[1]);
	str = argv[2];
	while (*str)
	{
		send_char(pid, *str);
		str++;
	}
	send_char(pid, '\n');
	return (0);
}
