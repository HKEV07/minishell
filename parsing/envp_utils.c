/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:36:38 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:39 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_env_count(t_env *e)
{
	size_t	count;

	count = 0;
	while (e)
	{
		count++;
		e = e->next;
	}
	return (count);
}

static char	*env_to_str(t_env *e)
{
	char	*str;
	size_t	len;

	str = malloc(e->key_len + e->value_len + 2);
	if (!str)
		return (str);
	len = ft_strncpy(str, e->key, e->key_len);
	len += ft_strncpy(str + len, "=", 1);
	len += ft_strncpy(str + len, e->value, e->value_len);
	str[len] = '\0';
	return (str);
}

int	should_skip_env(t_env *e)
{
	if (e->key_len == 1 && !ft_strncmp(e->key, "?", 1))
		return (1);
	return (0);
}

char	**env_to_envp(void)
{
	char	**envp;
	size_t	i;
	t_env	*e;

	e = g_state.g_env;
	envp = malloc(sizeof(char *) * (get_env_count(e) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (e)
	{
		if (!should_skip_env(e) && e->value && !e->skip)
		{
			envp[i] = env_to_str(e);
			if (!envp)
				return (NULL);
			i++;
		}
		e = e->next;
	}
	envp[i] = NULL;
	return (envp);
}

size_t	get_key_len(char *str)
{
	size_t	i;

	if (str[0] == '?')
		return (1);
	if (char_in_str(NUMS, str[0]))
		return (1);
	i = 0;
	while (str[i] && char_in_str(ENV_KEY_CHARS, str[i]))
		i++;
	return (i);
}
