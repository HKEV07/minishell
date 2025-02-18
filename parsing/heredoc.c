/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:15 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:17 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_heredoc	*get_last_heredoc(t_heredoc *head)
{
	while (head)
	{
		if (head->next == NULL)
			return (head);
		head = head->next;
	}
	return (head);
}

static t_heredoc	*new_heredoc(t_heredoc *prev, t_heredoc *next)
{
	t_heredoc	*heredoc;

	heredoc = malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	heredoc->prev = prev;
	heredoc->next = next;
	return (heredoc);
}

t_heredoc	*heredoc_pushback(t_heredoc **head, t_token *token)
{
	t_heredoc	*h;
	t_heredoc	*l;

	if (!*head)
	{
		h = new_heredoc(NULL, NULL);
		if (!h)
			return (NULL);
		*head = h;
	}
	else
	{
		l = get_last_heredoc(*head);
		h = new_heredoc(l, NULL);
		if (!h)
			return (NULL);
		l->next = h;
	}
	h->terminator = remove_quotes(token->token);
	if (token->prev->type == HEREDOC)
		h->expand = 1;
	else
		h->expand = 0;
	return (h);
}
