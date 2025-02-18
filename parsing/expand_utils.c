/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 17:21:11 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:47 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_len_with_env(char *str, size_t *i)
{
	size_t	j;
	size_t	key_len;
	char	*key;
	t_env	*e;

	j = 0;
	if (str[*i] == DOLLAR)
	{
		key_len = get_key_len(&str[*i + 1]);
		if (key_len)
		{
			key = ft_substr(&str[*i] + 1, 0, key_len);
			e = get_env_by_key(key, key_len);
			free(key);
			if (e && e->value)
				j = e->value_len;
		}
		else
			j++;
		*i += key_len + 1;
	}
	return (j);
}

static void	add_to_i_j(size_t *i, size_t *j, size_t add)
{
	if (i)
		*i += add;
	if (j)
		*j += add;
}

size_t	get_expanded_len(char *cmd, int quotes)
{
	size_t	i;
	size_t	len;
	size_t	j;
	int		dq;

	i = 0;
	j = 0;
	dq = 0;
	while (cmd[i])
	{
		if (quotes && !dq && cmd[i] && cmd[i] == S_QUOTE)
		{
			len = find_next(&cmd[i] + 1, S_QUOTE);
			add_to_i_j(&i, &j, len);
		}
		else if (cmd[i] && cmd[i] == D_QUOTE)
			dq = !dq;
		else if (cmd[i] && cmd[i] == DOLLAR)
		{
			j += get_len_with_env(cmd, &i);
			continue ;
		}
		add_to_i_j(&j, &i, 1);
	}
	return (j);
}
