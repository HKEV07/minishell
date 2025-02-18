/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:24 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:31 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	get_cmd_input(t_token *t)
{
	while (t)
	{
		if (t->type == HEREDOC || t->type == NHEREDOC)
			return (HEREDOC);
		else if (t->type == R_IN)
			return (R_IN);
		t = t->prev;
	}
	return (UNKNOWN);
}

void	cmd_check_and_move(t_command *cmd, t_token **tokens)
{
	if (!(*tokens)->next || (*tokens)->next->type == PIPE)
		cmd->last_redir = get_cmd_input(*tokens);
	*tokens = (*tokens)->next;
}

int	add_cmd_name(t_command *cmd, t_token *tokens)
{
	if (ft_strlen(tokens->token))
	{
		cmd->name = remove_quotes(tokens->token);
		cmd->argv = get_new_argv(cmd->name);
		if (!cmd->argv)
			return (0);
	}
	return (1);
}

static int	handle_rest(t_command *cmd, t_token **tokens)
{
	if ((*tokens)->type == R_IN || (*tokens)->type == R_OUT
		|| (*tokens)->type == APPEND)
	{
		*tokens = (*tokens)->next;
		if (!add_redir(cmd, *tokens))
			return (0);
	}
	else if ((*tokens)->type == HEREDOC || (*tokens)->type == NHEREDOC)
	{
		*tokens = (*tokens)->next;
		if (!heredoc_pushback(&cmd->heredoc, *tokens))
			return (0);
	}
	return (1);
}

t_command	*build_command(t_token *tokens)
{
	t_command	*cmd_head;
	t_command	*cmd;

	cmd_head = NULL;
	while (tokens)
	{
		if (!tokens->prev || tokens->prev->type == PIPE)
			if (!create_new_cmd(&cmd, &cmd_head))
				return (NULL);
		if (!cmd->name && tokens->type == WORD)
		{
			if (!add_cmd_name(cmd, tokens))
				return (NULL);
		}
		else if (tokens->type == WORD)
		{
			if (!add_argv(cmd, &tokens))
				return (NULL);
			continue ;
		}
		else if (!handle_rest(cmd, &tokens))
			return (NULL);
		cmd_check_and_move(cmd, &tokens);
	}
	return (cmd_head);
}
