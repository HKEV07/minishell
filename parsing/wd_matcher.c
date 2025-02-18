/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 16:49:44 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:47 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	*get_s_bitmap(char *str)
{
	size_t	wd_count;
	int		*bitmap;
	size_t	i;

	i = 0;
	wd_count = 0;
	while (str[i])
	{
		if (str[i] == WILDCARD)
			wd_count++;
		i++;
	}
	bitmap = _calloc(sizeof(int) * wd_count);
	if (!bitmap)
		return (NULL);
	i = 0;
	wd_count = 0;
	while (str[i])
	{
		if (str[i] == WILDCARD)
			bitmap[wd_count++] = is_in_quotes(str, i);
		i++;
	}
	return (bitmap);
}

size_t	get_star_pos(char *str, size_t pos)
{
	size_t	i;
	size_t	s_pos;

	i = 0;
	s_pos = 0;
	while (str[i])
	{
		if (str[i] == WILDCARD)
			s_pos++;
		if (i == pos)
			return (s_pos - 1);
		i++;
	}
	return (0);
}

static void	init_these(int **s_bitmap, char **match, char **s, char **wd_marker)
{
	*s_bitmap = get_s_bitmap(*match);
	*match = remove_quotes(ft_strdup(*match));
	*s = *match;
	*wd_marker = NULL;
}

static char	skip_wds(int *s_bitmap, char *match, char *start)
{
	while (*match == WILDCARD && !s_bitmap[get_star_pos(start, match - start)])
		match++;
	return (*match);
}

int	does_it_match(char *entry, char *match)
{
	char	*wd_marker;
	char	*start;
	int		*s_bitmap;
	char	l;

	init_these(&s_bitmap, &match, &start, &wd_marker);
	while (*entry)
	{
		if (*match == WILDCARD && !s_bitmap[get_star_pos(start, match - start)])
			wd_marker = match++;
		else if (*match == *entry)
		{
			entry++;
			match++;
		}
		else if (wd_marker)
		{
			++entry;
			match = wd_marker + 1;
		}
		else
			return (free(s_bitmap), free(start), 0);
	}
	l = skip_wds(s_bitmap, match, start);
	return (free(s_bitmap), free(start), (l == 0));
}
