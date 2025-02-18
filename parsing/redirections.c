/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:25 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:27 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*get_last_redir(void *head, t_token_type type)
{
	if (type == R_IN)
	{
		while (head)
		{
			if (((t_in_rediretion *)head)->next == NULL)
				return (head);
			head = ((t_in_rediretion *)head)->next;
		}
	}
	else
	{
		while (head)
		{
			if (((t_out_rediretion *)head)->next == NULL)
				return (head);
			head = ((t_out_rediretion *)head)->next;
		}
	}
	return (head);
}

static void	*new_redir(void *prev, t_token_type type, void *next)
{
	void	*r;

	if (type == R_IN)
	{
		r = malloc(sizeof(t_in_rediretion));
		if (!r)
			return (NULL);
		((t_in_rediretion *)r)->prev = prev;
		((t_in_rediretion *)r)->next = next;
	}
	else
	{
		r = malloc(sizeof(t_out_rediretion));
		if (!r)
			return (NULL);
		((t_out_rediretion *)r)->prev = prev;
		((t_out_rediretion *)r)->next = next;
	}
	return (r);
}

t_command	*redir_pushback(void **head, t_token_type type)
{
	void	*c;
	void	*l;

	if (!*head)
	{
		c = new_redir(NULL, type, NULL);
		if (!c)
			return (NULL);
		*head = c;
	}
	else
	{
		l = get_last_redir(*head, type);
		c = new_redir(l, type, NULL);
		if (!c)
			return (NULL);
		if (type == R_IN)
			((t_in_rediretion *)l)->next = c;
		else
			((t_out_rediretion *)l)->next = c;
	}
	return (c);
}

void	destroy_r_in(t_in_rediretion *r_in, t_token *tokens)
{
	t_in_rediretion	*next;
	t_token			*t;

	while (r_in)
	{
		next = r_in->next;
		t = map_str_to_token(tokens, r_in->filename);
		free(r_in->filename);
		if (t)
			t->token = NULL;
		free(r_in);
		r_in = next;
	}
}

void	destroy_r_out(t_out_rediretion *r_out, t_token *tokens)
{
	t_out_rediretion	*next;
	t_token				*t;

	while (r_out)
	{
		next = r_out->next;
		t = map_str_to_token(tokens, r_out->filename);
		free(r_out->filename);
		if (t)
			t->token = NULL;
		free(r_out);
		r_out = next;
	}
}
