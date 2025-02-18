/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:52:02 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:51 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wd_should_skip(char *match, char *entry)
{
	if (ft_strlen(entry) == 1 && entry[0] == '.')
		return (1);
	if (ft_strlen(entry) == 2 && entry[0] == '.' && entry[1] == '.')
		return (1);
	if (match[0] != '.' && entry[0] == '.')
		return (1);
	return (0);
}

static char	**alloc_v(char *match)
{
	size_t	r;
	char	**matches;

	r = count_wd_results(match);
	if (r)
	{
		matches = malloc((r + 1) * sizeof(char *));
		if (!matches)
			return (NULL);
	}
	else
		return (NULL);
	return (matches);
}

static char	**match_wildcard(char *match)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	size_t			j;

	matches = alloc_v(match);
	if (!matches)
		return (NULL);
	dir = opendir(CURRENT_DIR);
	if (!dir)
		return (NULL);
	j = 0;
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (wd_should_skip(match, entry->d_name))
			continue ;
		if (does_it_match(entry->d_name, match))
			matches[j++] = ft_strdup(entry->d_name);
	}
	closedir(dir);
	matches[j] = NULL;
	return (matches);
}

static int	add_wc_results(t_token **t, char **r, t_token **tokens)
{
	t_token	*tt;

	sort_keys(r, wd_keys_count(r));
	tt = inject_new_tokens(*t, r);
	free(r);
	if (!tt)
		return (0);
	if (tt->next)
		tt->next->prev = tt;
	if ((*t)->prev)
	{
		(*t)->prev->next = (*t)->next;
		(*t)->next->prev = (*t)->prev;
	}
	else
	{
		(*t)->next->prev = NULL;
		*tokens = (*t)->next;
	}
	free((*t)->token);
	free((*t));
	*t = tt;
	return (1);
}

t_token	*wildcard(t_token *tokens)
{
	t_token	*t;
	char	**r;

	t = tokens;
	while (t)
	{
		if (is_wildcard(t->token))
		{
			if (t->prev && (t->prev->type == R_IN || t->prev->type == R_OUT
					|| t->prev->type == APPEND)
				&& count_wd_results(t->token) > 1)
				return (tokens);
			r = match_wildcard(t->token);
			if (r)
				if (!add_wc_results(&t, r, &tokens))
					return (NULL);
		}
		t = t->next;
	}
	return (tokens);
}
