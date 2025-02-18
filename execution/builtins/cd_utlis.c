/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utlis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 14:55:26 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/11 14:55:28 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_last_ch(void)
{
	int		i;

	i = ft_strlen(g_state.pwd);
	if (g_state.pwd[i - 1] == '/')
		return (1);
	return (0);
}

char	*ft_add_back(char *path)
{
	char	*pwd;
	char	*old;

	if (check_last_ch())
		pwd = ft_strjoin(g_state.pwd, path);
	else
	{
		old = ft_strjoin(g_state.pwd, "/");
		pwd = ft_strjoin(old, path);
		free(old);
	}
	return (pwd);
}

int	handle_double_point(t_command *cmd)
{
	char	*path;
	char	*access_path;

	if (check_dir(cmd, cmd->argv[1]))
		return (EXIT_FAILURE);
	if (chdir(cmd->argv[1]) < 0)
		return (EXIT_FAILURE);
	access_path = getcwd(NULL, 0);
	if (!access_path)
	{
		path = ft_add_back(cmd->argv[1]);
		print_mes(cmd, NULL, 2);
		set_oldpwd(g_state.pwd);
		free(g_state.pwd);
		g_state.pwd = ft_strdup(path);
		free(path);
	}
	else if (chdir(access_path) == 0)
	{
		set_oldpwd(g_state.pwd);
		free(g_state.pwd);
		g_state.pwd = ft_strdup(access_path);
		free(access_path);
	}
	return (EXIT_SUCCESS);
}

void	set_pwd(char *pwd)
{
	t_env	*e;
	char	*s;
	char	*ss;

	e = get_env_by_key("PWD", 3);
	if (!e)
	{
		ss = ft_strdup("PWD=");
		s = ft_strjoin(ss, pwd);
		e = add_default_env(&g_state.g_env, s);
		free(ss);
		free(s);
	}
	else
	{
		free(e->value);
		e->value = ft_strdup(pwd);
		e->value_len = ft_strlen(e->value);
	}
}

void	set_oldpwd(char *oldpwd)
{
	t_env	*e;
	char	*s;
	char	*ss;

	e = get_env_by_key("OLDPWD", 6);
	if (!e)
	{
		ss = ft_strdup("OLDPWD=");
		s = ft_strjoin(ss, oldpwd);
		e = add_default_env(&g_state.g_env, s);
		free(ss);
		free(s);
	}
	else
	{
		free(e->value);
		e->value = ft_strdup(oldpwd);
		e->value_len = ft_strlen(e->value);
	}
}
