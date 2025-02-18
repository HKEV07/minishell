/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:40:46 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/09 19:44:07 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_command *cmd, int fd)
{
	char	*path;

	path = ft_strdup(g_state.pwd);
	if (!path)
	{
		cmd->set_exit_code(cmd, 1);
		return ;
	}
	write(fd, path, ft_strlen(path));
	write(fd, "\n", 2);
	free(path);
	cmd->set_exit_code(cmd, 0);
}
