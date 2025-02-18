/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:42 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:45 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	expand_env(char *new_str, char *str, size_t *i)
{
	size_t	j;
	size_t	key_len;
	char	*key;
	t_env	*e;

	j = 0;
	if (str[*i] == DOLLAR)
	{
		key_len = get_key_len(&str[*i + 1]);
		if (key_len)
		{
			key = ft_substr(&str[*i] + 1, 0, key_len);
			e = get_env_by_key(key, key_len);
			if (e && e->value)
				j += ft_strncpy(new_str + j, e->value, e->value_len);
			*i += key_len + 1;
			free(key);
		}
		else
			new_str[j++] = str[(*i)++];
	}
	return (j);
}

static size_t	do_loop(char *str, char *new_str, int quotes, int *dq)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (quotes && !*dq && str[i] && str[i] == S_QUOTE)
		{
			if (handle_sq(str, new_str, &j, &i))
				continue ;
		}
		else if (str[i] && str[i] == D_QUOTE)
			*dq = !*dq;
		else if (str[i] == DOLLAR)
		{
			if (!*dq && char_in_str("\"'", str[i + 1]) && quotes)
				i++;
			else
				j += expand_env(new_str + j, str, &i);
			continue ;
		}
		new_str[j++] = str[i++];
	}
	return (j);
}

char	*expand_str(char *str, int quotes)
{
	char	*new_str;
	size_t	j;
	int		dq;

	new_str = malloc(get_expanded_len(str, quotes) + 1);
	if (!new_str)
		return (NULL);
	j = 0;
	dq = 0;
	j += do_loop(str, new_str, quotes, &dq);
	new_str[j] = '\0';
	return (new_str);
}

static int	exapnd_token(t_token *tokens, char *token)
{
	char	*x_token;
	char	**s_tokens;

	x_token = expand_str(tokens->token, 1);
	s_tokens = ft_split(x_token);
	tokens->token = s_tokens[0];
	if (s_tokens[1])
		if (!inject_new_tokens(tokens, s_tokens + 1))
			return (0);
	free(x_token);
	free(s_tokens);
	free(token);
	if (!tokens->token)
		return (0);
	return (1);
}

int	expand(t_token *tokens)
{
	char	*token;
	t_token	*next;

	while (tokens)
	{
		token = tokens->token;
		next = tokens->next;
		if (tokens->prev && (tokens->prev->type == HEREDOC
				|| tokens->prev->type == NHEREDOC))
		{
			remove_d_q(tokens->token);
			tokens = next;
			continue ;
		}
		if (!exapnd_token(tokens, token))
			return (0);
		tokens = next;
	}
	return (1);
}
