/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:49:13 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:49:14 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_value_next(t_env *ret, char *e, int append)
{
	char	*oldv;
	char	*v;

	if (append)
	{
		if (!ret->value)
			ret->value = ft_strdup("");
		oldv = ret->value;
		v = get_value(e);
		ret->value = ft_strjoin(ret->value, v);
		free(oldv);
		free(v);
	}
	else
	{
		oldv = ret->value;
		ret->value = get_value(e);
		free(oldv);
	}
	ret->value_len = ft_strlen(ret->value);
}

static void	set_value(size_t e_index, t_env *ret, char *e, int append)
{
	if (!e_index)
		return ;
	else if (e_index == (ret->key_len - 1))
	{
		free(ret->value);
		ret->value = ft_strdup("");
		ret->value_len = 0;
	}
	else
		set_value_next(ret, e, append);
}

void	_export(char *e, int *code)
{
	t_env		*ret;
	char		*key;
	size_t		e_index;
	size_t		len;
	int			append;

	append = 0;
	key = get_key(e);
	if (invalid_key(key, e, code, &append))
		return ;
	len = ft_strlen(key);
	ret = get_env_by_key(key, len);
	if (!ret)
	{
		ret = env_pushback(&g_state.g_env);
		if (!ret)
			return ;
		ret->key = key;
		ret->key_len = len;
	}
	else
		free(key);
	e_index = get_index_of(e, '=');
	set_value(e_index, ret, e, append);
}
