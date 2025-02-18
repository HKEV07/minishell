/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _ft_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 17:33:27 by ibenaait          #+#    #+#             */
/*   Updated: 2023/07/31 23:39:36 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_alloc_word(char const *s, char sep, size_t *wc)
{
	size_t	pos;
	size_t	count;
	char	**sp;

	if (!s)
		return (0);
	pos = 0;
	count = 0;
	while (s[pos])
	{
		while (s[pos] != '\0' && s[pos] == sep)
			pos++;
		if (s[pos] && s[pos] != sep)
			count++;
		while (s[pos] && s[pos] != sep)
			pos++;
	}
	*wc = count;
	sp = malloc(sizeof(char *) * (*wc + 1));
	return (sp);
}

static char	*ft_al_word_sub(char const *s, char sep)
{
	size_t	word_len;
	char	*wp;

	word_len = 0;
	while (s[word_len] && s[word_len] != sep)
		word_len++;
	wp = ft_substr(s, 0, word_len);
	return (wp);
}

static char	**ft_free(char **s, size_t max)
{
	size_t	i;

	i = 0;
	while (i <= max)
	{
		if (s[i])
			free(s[i]);
		i++;
	}
	free(s);
	return (0);
}

static char	**ft_check_for_alloc(char **s, size_t wc, size_t max)
{
	size_t	i;

	i = 0;
	while (max < wc && i <= max)
	{
		if (!s[i])
			return (ft_free(s, max));
		i++;
	}
	s[wc] = 0;
	return (s);
}

char	**_ft_split(char const *s, char c)
{
	size_t	wc;
	char	**sp;
	size_t	pos;
	size_t	i;

	sp = ft_alloc_word(s, c, &wc);
	if (!sp)
		return (0);
	pos = 0;
	i = 0;
	while (i < wc)
	{
		while (s[pos] && s[pos] == c)
			pos++;
		if (s[pos] && s[pos] != c)
		{
			sp[i] = ft_al_word_sub(s + pos, c);
			if (!sp[i])
				break ;
			i++;
		}
		while (s[pos] && s[pos] != c)
			pos++;
	}
	return (ft_check_for_alloc(sp, wc, i));
}
