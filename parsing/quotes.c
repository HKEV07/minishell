/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:19 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:21 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_d_q(char *str)
{
	size_t	i;
	size_t	j;
	int		dq;

	i = 0;
	j = 0;
	dq = 0;
	while (str[i])
	{
		if (!dq && str[i] && str[i] == S_QUOTE)
			handle_sq(str, str, &j, &i);
		else if (str[i] && str[i] == D_QUOTE)
		{
			str[j++] = str[i++];
			dq = !dq;
		}
		else if (!dq && str[i] == DOLLAR && char_in_str("\"'", str[i + 1]))
			i++;
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}

int	handle_sq(char *str, char *new_str, size_t *j, size_t *i)
{
	size_t	strip_count;

	strip_count = find_next(&str[*i] + 1, S_QUOTE);
	if (strip_count)
	{
		*j += ft_strncpy(new_str + *j, str + *i, strip_count + 1);
		*i += strip_count + 1;
		return (1);
	}
	return (0);
}

int	all_empty(char *str)
{
	size_t	i;
	size_t	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] == S_QUOTE && str[i + 1] == S_QUOTE) || \
			(str[i] == D_QUOTE && str[i + 1] == D_QUOTE))
		{
			i += 2;
			continue ;
		}
		count++;
		i++;
	}
	return (count == 0);
}

char	*remove_quotes(char *str)
{
	size_t	i;
	size_t	j;
	char	q;

	i = 0;
	j = 0;
	q = 0;
	while (str[i])
	{
		if (!q && (str[i] == S_QUOTE || str[i] == D_QUOTE))
		{
			q = str[i++];
			continue ;
		}
		if (q && str[i] == q)
		{
			i++;
			q = 0;
			continue ;
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
	return (str);
}
