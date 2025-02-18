/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:27 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:29 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	destroy_heredoc(t_heredoc *heredoc, t_token *tokens)
{
	t_heredoc	*next;
	t_token		*t;

	while (heredoc)
	{
		next = heredoc->next;
		t = map_str_to_token(tokens, heredoc->terminator);
		free(heredoc->terminator);
		if (t)
			t->token = NULL;
		free(heredoc);
		heredoc = next;
	}
}

static void	destroy_argv(char **argv, t_token *tokens)
{
	size_t	i;
	t_token	*t;

	i = 1;
	while (argv && argv[i])
	{
		t = map_str_to_token(tokens, argv[i]);
		free(argv[i]);
		if (t)
			t->token = NULL;
		i++;
	}
	if (argv)
		free(argv);
}

static void	destroy_envp(char **envp, t_token *tokens)
{
	size_t	i;
	t_token	*t;

	i = 0;
	while (envp && envp[i])
	{
		t = map_str_to_token(tokens, envp[i]);
		free(envp[i]);
		if (t)
			t->token = NULL;
		i++;
	}
	if (envp)
		free(envp);
}

void	restore_stdin(void)
{
	dup2(g_state.duped_stdin, 0);
	close(g_state.duped_stdin);
	g_state.special_sigint = 0;
}

void	free_all(t_token *tokens, t_command *cmd)
{
	t_command	*next;
	t_token		*t;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->name)
		{
			t = map_str_to_token(tokens, cmd->name);
			free(cmd->name);
			if (t)
				t->token = NULL;
		}
		destroy_argv(cmd->argv, tokens);
		destroy_r_in(cmd->in, tokens);
		destroy_r_out(cmd->out, tokens);
		destroy_heredoc(cmd->heredoc, tokens);
		destroy_envp(cmd->envp, tokens);
		free(cmd);
		cmd = next;
	}
	destroy_tokens(tokens);
	restore_stdin();
}
