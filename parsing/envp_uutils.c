/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_uutils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:36:39 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:40 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_by_key(char *key, size_t len)
{
	t_env	*e;

	e = g_state.g_env;
	while (e)
	{
		if (e->key_len == len && !ft_strncmp(e->key, key, len))
			return (e);
		e = e->next;
	}
	return (NULL);
}

void	delete_env_by_key(char *key, size_t len)
{
	t_env	*e;
	t_env	*prev;
	t_env	*next;

	e = get_env_by_key(key, len);
	if (e)
	{
		prev = e->prev;
		next = e->next;
		if (prev)
			prev->next = next;
		else
			g_state.g_env = next;
		if (next)
			next->prev = prev;
		free(e->key);
		if (e->value)
			free(e->value);
		free(e);
	}
}

char	**get_envp(t_command *self)
{
	size_t	i;

	if (self->envp)
	{
		i = 0;
		while (self->envp[i])
			free(self->envp[i++]);
		free(self->envp);
	}
	self->envp = env_to_envp();
	return (self->envp);
}

char	*get_env_path(void)
{
	t_env	*e;

	e = get_env_by_key("PATH", 4);
	if (e)
		return (e->value);
	else
		return ("");
}

t_env	*env_add_shlvl(void)
{
	t_env	*e;
	char	*p;

	e = get_env_by_key("SHLVL", 5);
	if (e)
	{
		p = e->value;
		e->value = ft_itoa(ft_atoi(p) + 1);
		if (!e->value)
			return (NULL);
		e->value_len = ft_strlen(e->value);
		free(p);
	}
	else
	{
		e = env_pushback(&g_state.g_env);
		if (!e)
			return (NULL);
		e->key = ft_strdup("SHLVL");
		e->key_len = 5;
		e->value = ft_itoa(1);
		e->value_len = 1;
	}
	return (g_state.g_env);
}
