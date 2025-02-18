/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rs_checks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:29 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:30 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	contains_raw_whitespaces(char *token)
{
	size_t	i;
	char	q;

	i = 0;
	q = 0;
	while (token[i])
	{
		if (!q && char_in_str("\"'", token[i]))
			q = token[i];
		else if (q && q == token[i])
			q = 0;
		else if (!q && (token[i] == ' ' || token[i] == '\t'))
			return (1);
		i++;
	}
	return (0);
}

int	invalid_filename_expansion(char *token)
{
	if (!ft_strlen(token))
		return (1);
	else if (contains_raw_whitespaces(token))
		return (1);
	return (0);
}

int	invalid_redir(char *filename, char *original, t_token_type type)
{
	if (invalid_filename_expansion(filename))
	{
		write(2, "minishell: ", 11);
		write(2, original, ft_strlen(original));
		write(2, ": ambiguous redirect\n", 21);
		return (1);
	}
	else if (count_wd_results(filename) > 1)
	{
		write(2, "minishell: ", 11);
		write(2, original, ft_strlen(original));
		write(2, ": ambiguous redirect\n", 21);
		return (1);
	}
	else if (type != R_IN && rout_check(filename))
		return (1);
	return (0);
}

void	check_for_invalid_rs(t_token *tokens)
{
	t_token	*t;
	char	*ex;

	t = tokens;
	while (t)
	{
		if (is_redir(t->type))
		{
			ex = expand_str(t->next->token, 1);
			if (invalid_redir(ex, t->next->token, t->type))
				t->ambiguous = 1;
			else if (t->type == R_IN && rin_check(t->next->token))
				t->ambiguous = 1;
			free(ex);
			if (t->ambiguous)
			{
				remove_all_following_rs(&t, t);
				continue ;
			}
		}
		t = t->next;
	}
}
