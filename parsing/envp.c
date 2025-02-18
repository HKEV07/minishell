/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:40 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:37 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*add_pwd(void)
{
	t_env	*e;

	e = get_env_by_key("PWD", 3);
	if (!e)
	{
		e = env_pushback(&g_state.g_env);
		if (!e)
			return (NULL);
		e->key = ft_strdup("PWD");
		e->key_len = 3;
		e->value = getcwd(NULL, 0);
		if (!e->value)
			return (NULL);
		e->value_len = ft_strlen(e->value);
	}
	return (e);
}

static t_env	*add_path(void)
{
	t_env	*e;

	e = get_env_by_key("PATH", 4);
	if (!e)
	{
		e = env_pushback(&g_state.g_env);
		if (!e)
			return (NULL);
		e->key = ft_strdup("PATH");
		e->key_len = 4;
		e->value = ft_strdup(DEFAULT_PATH);
		e->value_len = ft_strlen(e->value);
		e->skip = 1;
	}
	return (e);
}

static t_env	*check_for_defaults(void)
{
	t_env	*e;

	if (!add_pwd())
		return (NULL);
	e = get_env_by_key("_", 1);
	if (!e)
	{
		e = env_pushback(&g_state.g_env);
		if (!e)
			return (NULL);
		e->key = ft_strdup("_");
		e->key_len = 1;
		e->value = ft_strdup("/usr/bin/env");
		e->value_len = ft_strlen(e->value);
	}
	if (!add_path())
		return (NULL);
	g_state.pwd = ft_strdup(get_env_by_key("PWD", 3)->value);
	return (env_add_shlvl());
}

t_env	*build_env(char *envp[])
{
	size_t	i;
	t_env	*envh;
	t_env	*ret;

	i = 0;
	envh = NULL;
	ret = add_default_env(&envh, "?=0");
	if (!ret)
		return (NULL);
	while (envp[i])
	{
		ret = env_pushback(&envh);
		if (!ret)
			return (NULL);
		ret->key = get_key(envp[i]);
		ret->key_len = ft_strlen(ret->key);
		ret->value = get_value(envp[i]);
		ret->value_len = ft_strlen(ret->value);
		i++;
	}
	g_state.g_env = envh;
	signal(SIGQUIT, handle_sigquit);
	return (check_for_defaults());
}
