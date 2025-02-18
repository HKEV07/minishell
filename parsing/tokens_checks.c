/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:43 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:45 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_heredoc_count(t_token *t)
{
	size_t	count;

	while (t)
	{
		if (!t->prev || t->type == PIPE)
			count = 0;
		if (t->type == HEREDOC)
			count++;
		if (count > 16)
		{
			write(2, "minishell: maximum here-document count exceeded\n", 48);
			exit(2);
		}
		t = t->next;
	}
}

int	syntax_error(t_token *tokens)
{
	t_token	*t;

	t = tokens;
	while (t)
	{
		if (t->type == UNKNOWN)
			return (1);
		t = t->next;
	}
	t = tokens;
	while (t)
	{
		if ((t->type == R_IN || t->type == R_OUT || t->type == APPEND
				|| t->type == HEREDOC))
			if (!t->next || t->next->type != WORD)
				return (1);
		if (t->type == PIPE && ((!t->prev || !t->next) || (t->prev->type == PIPE
					|| t->next->type == PIPE)))
			return (1);
		t = t->next;
	}
	check_heredoc_count(tokens);
	check_for_invalid_rs(tokens);
	return (0);
}

int	check_q_error(char *str)
{
	size_t	i;
	char	q;

	i = 0;
	q = 0;
	while (str[i])
	{
		if (!q && (str[i] == S_QUOTE || str[i] == D_QUOTE))
		{
			q = str[i];
			i++;
		}
		else if (q && str[i] == q)
		{
			q = 0;
			i++;
		}
		else
			i++;
	}
	return (q != 0);
}
