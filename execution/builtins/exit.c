/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:01:01 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 13:34:33 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_mes_exit(t_command *cmd, int i)
{
	if (!i)
	{
		write(2, "minishell: exit: ", 17);
		write(2, cmd->argv[1], ft_strlen(cmd->argv[1]));
		write(2, ": numeric argument required\n", 28);
		exit(255);
	}
	else if (i)
	{
		write(2, "minishell: exit: ", 17);
		write(2, "too many arguments\n", 20);
	}
}

static int	pl_m(t_command *cmd, size_t j, int c)
{
	unsigned long	p;

	p = 0;
	while (cmd->argv[1][j])
	{
		if (cmd->argv[1][j] >= '0' && cmd->argv[1][j] <= '9' && p < INT_LL)
			p = (p * 10) + (cmd->argv[1][j] - 48);
		else
			print_mes_exit(cmd, 0);
		j++;
	}
	if (p > INT_LL)
		print_mes_exit(cmd, 0);
	if (cmd->argv[2])
	{
		print_mes_exit(cmd, 1);
		return (1);
	}
	exit((c * p) % 256);
	return (0);
}

int	ft_check_atoi_exit(t_command *cmd, char *str)
{
	size_t	i;
	int		c;
	int		s;

	i = 0;
	c = 1;
	s = 0;
	while (str[i] == '\f' || str[i] == '\t' || str[i] == ' ' \
	|| str[i] == '\n' || str[i] == '\r' || str[i] == '\v')
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			c = -1;
		i++;
	}
	s = pl_m(cmd, i, c);
	if (s)
		return (1);
	return (0);
}

void	exit_cmd(t_command *cmd)
{
	if (!cmd->next && !cmd->prev)
		write(1, "exit\n", 5);
	if (cmd->argv[1])
	{
		if (ft_check_atoi_exit(cmd, cmd->argv[1]))
		{
			cmd->set_exit_code(cmd, 1);
			return ;
		}
	}
	exit(0);
}
