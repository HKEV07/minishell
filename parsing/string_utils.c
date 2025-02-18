/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:37 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:38 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_index_of(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

int	char_in_str(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (1);
		i++;
	}
	return (0);
}

size_t	find_next_until(char *str, char c, size_t end)
{
	size_t	i;

	i = 0;
	while (str[i] && i < end)
	{
		if (str[i] == c)
			return (i + 1);
		i++;
	}
	return (0);
}

size_t	count_c_until_diff(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] == c)
		i++;
	return (i);
}

size_t	find_next_diff(char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != c)
			return (i);
		i++;
	}
	return (0);
}
