/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:53:25 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 22:55:36 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_if_can_print(char *str)
{
	size_t	i;
	size_t	j;
	size_t	n;

	n = 0;
	i = 0;
	j = 0;
	if (str != NULL)
	{
		if (!ft_strncmp(str, "-n", 2))
		{
			j += 2;
			i = j;
			n = 1;
		}
		while (str[i])
		{
			if (!ft_strncmp(&str[i], "n", 1))
				j++;
			i++;
		}
		if (ft_strlen(str) == j && n == 1)
			return (1);
	}
	return (0);
}

void	echo(t_command *cmd, int fd)
{
	size_t		i;
	int			no_nl;
	int			n;

	n = 0;
	i = 1;
	no_nl = ft_if_can_print(cmd->argv[1]);
	while (cmd->argv[i])
	{
		if (!ft_if_can_print(cmd->argv[i]))
		{
			write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
			n = 1;
		}
		else if (n == 1)
			write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
		i++;
		if (cmd->argv[i] != NULL && n == 1)
			write(fd, " ", 1);
	}
	if (!no_nl)
		write(fd, "\n", 1);
	cmd->set_exit_code(cmd, 0);
}
