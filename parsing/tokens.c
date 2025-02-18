/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:52:00 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:42 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_token_type(char *token, char *next)
{
	size_t	len;

	len = ft_strlen(token);
	if (len == 1 && !ft_strncmp("|", token, 1))
		return (PIPE);
	else if (len == 1 && !ft_strncmp("<", token, 1))
		return (R_IN);
	else if (len == 1 && !ft_strncmp(">", token, 1))
		return (R_OUT);
	else if (len == 2 && !ft_strncmp("<<", token, 2))
	{
		if (next && (char_in_str(next, D_QUOTE) || char_in_str(next, S_QUOTE)))
			return (NHEREDOC);
		return (HEREDOC);
	}
	else if (len == 2 && !ft_strncmp(">>", token, 2))
		return (APPEND);
	else if (token[0] == R_IN || token[0] == R_OUT || token[0] == PIPE)
		return (UNKNOWN);
	else
		return (WORD);
}

t_token	*populate_tokens(char *tokens[])
{
	t_token	*head;
	t_token	*t;
	char	*next;
	size_t	i;

	head = NULL;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i + 1])
			next = tokens[i + 1];
		else
			next = NULL;
		t = token_pushback(&head, tokens[i], get_token_type(tokens[i], next));
		if (!t)
			return (NULL);
		i++;
	}
	free(tokens);
	return (head);
}

void	*inject_new_tokens(t_token *token, char *tokens[])
{
	t_token	*t;
	t_token	*prev;
	t_token	*next;
	size_t	i;

	prev = token;
	next = token->next;
	i = 0;
	while (tokens[i])
	{
		t = new_token_node(prev, tokens[i], WORD, next);
		if (!t)
			return (NULL);
		prev->next = t;
		prev = t;
		i++;
	}
	return (t);
}

t_token	*map_str_to_token(t_token *tokens, char *str)
{
	while (tokens)
	{
		if (tokens->token == str)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}
