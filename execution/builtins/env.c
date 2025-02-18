/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:53:45 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 19:54:00 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_cmd(t_command *cmd, int fd)
{
	size_t		i;

	i = 0;
	cmd->get_envp(cmd);
	while (cmd->envp[i])
	{
		write(fd, cmd->envp[i], ft_strlen(cmd->envp[i]));
		write(fd, "\n", 1);
		i++;
	}
}
