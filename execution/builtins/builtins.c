/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:47:40 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 13:34:49 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	size_t		len;

	len = ft_strlen(cmd_name);
	if ((ft_strlen(ECHO_CMD) == len && !ft_strncmp(ECHO_CMD, cmd_name, len)) || \
		(ft_strlen(PWD_CMD) == len && !ft_strncmp(PWD_CMD, cmd_name, len)) || \
		(ft_strlen(CD_CMD) == len && !ft_strncmp(CD_CMD, cmd_name, len)) || \
		(ft_strlen(EXPORT_CMD) == len && \
		!ft_strncmp(EXPORT_CMD, cmd_name, len)) || \
		(ft_strlen(ENV_CMD) == len && \
		!ft_strncmp(ENV_CMD, cmd_name, len)) || \
		(ft_strlen(UNSET_CMD) == len && \
		!ft_strncmp(UNSET_CMD, cmd_name, len)) || \
		(ft_strlen(EXIT_CMD) == len && !ft_strncmp(EXIT_CMD, cmd_name, len)))
		return (1);
	return (0);
}

void	handle_builtin(t_command *cmd, int fd)
{
	size_t		len;

	len = ft_strlen(cmd->name);
	if (ft_strlen(ECHO_CMD) == len && !ft_strncmp(ECHO_CMD, cmd->name, len))
		echo(cmd, fd);
	else if (ft_strlen(PWD_CMD) == len && !ft_strncmp(PWD_CMD, cmd->name, len))
		pwd(cmd, fd);
	else if (ft_strlen(CD_CMD) == len && \
	!ft_strncmp(CD_CMD, cmd->name, len))
	{
		cmd->exit_code = cd(cmd);
		cmd->set_exit_code(cmd, cmd->exit_code);
	}
	else if (ft_strlen(EXPORT_CMD) == len && \
	!ft_strncmp(EXPORT_CMD, cmd->name, len))
		export(cmd, fd);
	else if (ft_strlen(ENV_CMD) == len && !ft_strncmp(ENV_CMD, cmd->name, len))
		env_cmd(cmd, fd);
	else if (ft_strlen(UNSET_CMD) == len && \
	!ft_strncmp(UNSET_CMD, cmd->name, len))
		unset(cmd);
	else if (ft_strlen(EXIT_CMD) == len && \
	!ft_strncmp(EXIT_CMD, cmd->name, len))
		exit_cmd(cmd);
}
