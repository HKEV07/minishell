/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utlis_fun.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:39:25 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 22:20:15 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_find_path(char *cmd)
{
	char	*path;
	char	**path_str;
	char	*p;

	if (is_not_cmd(cmd))
	{
		path_str = _ft_split(get_env_path(), ':');
		while (*path_str)
		{
			p = ft_strjoin(*path_str, "/");
			path = ft_strjoin(p, cmd);
			if (access(path, X_OK & F_OK) == 0)
				return (free(p), path);
			free(path);
			free(p);
			path_str++;
		}
		return (NULL);
	}
	return (cmd);
}

int	is_dir(char *filename)
{
	struct stat		path_stat;

	if (stat(filename, &path_stat))
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	is_ambg(t_command *cmd)
{
	t_in_rediretion		*in;
	t_out_rediretion	*out;

	in = cmd->in;
	out = cmd->out;
	while (in)
	{
		if (in->ambiguous)
			return (1);
		in = in->next;
	}
	while (out)
	{
		if (out->ambiguous)
			return (1);
		out = out->next;
	}
	return (0);
}

void	ft_execve_cmd_one(t_command *cmd)
{
	if (is_dir(cmd->name))
	{
		print_mes(cmd, ": is a directory\n", 1);
		exit (126);
	}
	else if (access(cmd->name, F_OK) < 0)
	{
		print_mes(cmd, ": No such file or directory\n", 1);
		exit(127);
	}
	else if (access(cmd->name, X_OK) < 0)
	{
		print_mes(cmd, ": Permission denied\n", 1);
		exit (126);
	}
	else
		execve(cmd->name, cmd->argv, cmd->get_envp(cmd));
	exit (0);
}

void	ft_execve_cmd(t_command *cmd)
{
	if (!cmd->name)
		exit(is_ambg(cmd));
	else if (!is_not_cmd(cmd->name) || ft_strlen(get_env_path()) == 0)
		ft_execve_cmd_one(cmd);
	else if (execve(ft_find_path(cmd->name), \
	cmd->argv, cmd->get_envp(cmd)) < 0)
	{
		print_mes(cmd, ": command not found\n", 1);
		exit (127);
	}
}
