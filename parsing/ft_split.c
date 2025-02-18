/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:43 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:48 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	do_loop(char *s, size_t *pos)
{
	while (s[*pos] && !is_whitespace(s[*pos]))
	{
		if (s[*pos] && s[*pos] == S_QUOTE)
			*pos += find_next(&s[*pos] + 1, S_QUOTE);
		else if (s[*pos] && s[*pos] == D_QUOTE)
			*pos += find_next(&s[*pos] + 1, D_QUOTE);
		(*pos)++;
	}
}

static char	**allocate_wordsv(char *s, size_t *words_count, size_t *i)
{
	size_t	pos;
	size_t	count;
	char	**wordsv;

	*i = 0;
	if (!s)
		return (NULL);
	pos = 0;
	count = 0;
	while (s[pos])
	{
		while (s[pos] && is_whitespace(s[pos]))
			pos++;
		if (s[pos] && !is_whitespace(s[pos]))
			count++;
		do_loop(s, &pos);
	}
	if (!count)
		count++;
	*words_count = count;
	wordsv = malloc(sizeof(char *) * (*words_count + 1));
	return (wordsv);
}

static char	**check_for_alloc_error(char **wordsv, size_t wc, size_t max)
{
	size_t	i;

	i = 0;
	while (max < wc && i <= max)
	{
		if (!wordsv[i])
			return (ft_split_cleanup(wordsv, max));
		i++;
	}
	wordsv[wc] = 0;
	return (wordsv);
}

static char	**handle_empty_str(char *s)
{
	char	**wv;

	wv = malloc(sizeof(char *) * 2);
	if (!wv)
		return (NULL);
	wv[0] = ft_strdup(s);
	wv[1] = NULL;
	return (wv);
}

char	**ft_split(char *s)
{
	size_t	words_count;
	char	**wordsv;
	size_t	pos;
	size_t	start;
	size_t	i;

	if (!ft_strlen(s))
		return (handle_empty_str(s));
	wordsv = allocate_wordsv(s, &words_count, &i);
	if (!wordsv)
		return (0);
	pos = 0;
	while (s[pos])
	{
		while (s[pos] && is_whitespace(s[pos]))
			pos++;
		start = pos;
		do_loop(s, &pos);
		wordsv[i] = ft_substr(s, start, pos - start);
		if (!wordsv[i++])
			break ;
		while (s[pos] && is_whitespace(s[pos]))
			pos++;
	}
	return (check_for_alloc_error(wordsv, words_count, i));
}
