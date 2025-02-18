/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env__.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:36:35 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:36 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*get_last_env(t_env *head)
{
	while (head)
	{
		if (head->next == NULL)
			return (head);
		head = head->next;
	}
	return (head);
}

t_env	*env_pushback(t_env **head)
{
	t_env	*last_p;

	if (!*head)
	{
		last_p = _calloc(sizeof(t_env));
		if (!last_p)
			return (NULL);
		last_p->next = NULL;
		last_p->prev = NULL;
		*head = last_p;
	}
	else
	{
		last_p = get_last_env(*head);
		last_p->next = _calloc(sizeof(t_env));
		if (!last_p->next)
			return (NULL);
		last_p->next->prev = last_p;
		last_p = last_p->next;
		last_p->next = NULL;
	}
	last_p->value = NULL;
	return (last_p);
}

char	*get_key(char *e)
{
	size_t	i;

	i = get_index_of(e, '=');
	if (!i)
		i = ft_strlen(e);
	return (ft_substr(e, 0, i));
}

char	*get_value(char *e)
{
	size_t	i;

	i = get_index_of(e, '=');
	return (ft_substr(e, i + 1, (ft_strlen(e) - i)));
}

t_env	*add_default_env(t_env **env, char *e)
{
	t_env	*ret;

	ret = env_pushback(env);
	if (!ret)
		return (NULL);
	ret->key = get_key(e);
	ret->key_len = ft_strlen(ret->key);
	ret->value = get_value(e);
	ret->value_len = ft_strlen(ret->value);
	return (ret);
}
