/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:48:12 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/11 14:55:34 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir(t_command *cmd, char *path)
{
	if (ft_strlen(path) == 0)
		return (EXIT_SUCCESS);
	else if (access(path, F_OK) < 0)
		return (print_mes(cmd, \
		": No such file or directory\n", 0), EXIT_FAILURE);
	else if (!is_dir(path))
		return (print_mes(cmd, ": Not a directory\n", 0), EXIT_FAILURE);
	else if (access(path, X_OK) < 0)
		return (print_mes(cmd, ": Permission denied\n", 0), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*get_env_path_home(void)
{
	t_env	*e;

	e = get_env_by_key("HOME", 4);
	if (e)
		return (e->value);
	else
		return ("");
}

void	print_mes(t_command *cmd, char *str, int i)
{
	if (i == 0 || i == 1)
	{
		write(2, "minishell: ", 11);
		write(2, cmd->name, ft_strlen(cmd->name));
	}
	if (i == 0)
	{
		write(2, ": ", 2);
		write(2, cmd->argv[1], ft_strlen(cmd->argv[1]));
		write(2, str, ft_strlen(str));
	}
	else if (i == 1)
		write(2, str, ft_strlen(str));
	else if (i == 2)
	{
		write(2, cmd->name, ft_strlen(cmd->name));
		write(2, ": error retrieving current directory: getcwd", 45);
		write(2, ": cannot access parent directories", 35);
		write(2, ": No such file or directory\n", 29);
	}
}

int	handle_home(t_command *cmd, int i)
{
	char	*home;

	home = ft_strdup(get_env_path_home());
	if (check_dir(cmd, home))
		return (free(home), EXIT_FAILURE);
	if ((!ft_strlen(home) && i))
	{
		free(home);
		home = ft_strdup("/Users/ibenaait");
	}
	if (chdir(home) != 0)
		return (print_mes(cmd, ": HOME not set\n", 1), free(home), EXIT_FAILURE);
	set_oldpwd(g_state.pwd);
	free(g_state.pwd);
	g_state.pwd = ft_strdup(home);
	return (free(home), EXIT_SUCCESS);
}

int	cd(t_command *cmd)
{
	if (cmd->argv[1] == NULL)
	{
		if (handle_home(cmd, 0))
			return (EXIT_FAILURE);
	}
	else if (ft_strlen(cmd->argv[1]) == 1 && !ft_strncmp("~", cmd->argv[1], 1))
	{
		if (handle_home(cmd, 1))
			return (EXIT_FAILURE);
	}
	else
	{
		if (handle_double_point(cmd))
			return (EXIT_FAILURE);
	}
	set_pwd(g_state.pwd);
	return (EXIT_SUCCESS);
}
