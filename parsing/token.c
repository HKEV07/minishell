/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:39 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:41 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*get_last_token(t_token *head)
{
	while (head)
	{
		if (head->next == NULL)
			return (head);
		head = head->next;
	}
	return (head);
}

t_token	*new_token_node(t_token *prev, char *token, t_token_type type,
		t_token *next)
{
	t_token	*token_node;

	token_node = _calloc(sizeof(t_token));
	if (!token_node)
		return (NULL);
	token_node->prev = prev;
	token_node->type = type;
	token_node->token = token;
	token_node->next = next;
	return (token_node);
}

t_token	*token_pushback(t_token **head, char *token, t_token_type type)
{
	t_token	*t;
	t_token	*l;

	if (!*head)
	{
		t = new_token_node(NULL, token, type, NULL);
		if (!t)
			return (NULL);
		*head = t;
	}
	else
	{
		l = get_last_token(*head);
		t = new_token_node(l, token, type, NULL);
		if (!t)
			return (NULL);
		l->next = t;
	}
	return (t);
}

void	destroy_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		if (tokens->token)
			free(tokens->token);
		free(tokens);
		tokens = next;
	}
}
