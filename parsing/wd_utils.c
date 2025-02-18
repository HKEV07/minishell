/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 17:04:19 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:48 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_wd_results(char *match)
{
	struct dirent	*entry;
	size_t			count;
	DIR				*dir;

	dir = opendir(CURRENT_DIR);
	if (!dir)
		return (0);
	count = 0;
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (wd_should_skip(match, entry->d_name))
			continue ;
		if (does_it_match(entry->d_name, match))
			count++;
	}
	closedir(dir);
	return (count);
}

size_t	wd_keys_count(char **v)
{
	size_t	i;

	i = 0;
	while (v[i])
		i++;
	return (i);
}

int	is_wildcard(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] && (str[i] == S_QUOTE || str[i] == D_QUOTE))
		{
			i += find_next(&str[i] + 1, str[i]) + 1;
			continue ;
		}
		if (str[i] == WILDCARD)
			return (1);
		i++;
	}
	return (0);
}
