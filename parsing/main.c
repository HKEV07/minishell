/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:50 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:18 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_type(t_token_type t)
{
	if (t == WORD)
		return ("WORD");
	else if (t == R_IN)
		return ("R_IN");
	else if (t == R_OUT)
		return ("R_OUT");
	else if (t == UNKNOWN)
		return ("UNKNOWN");
	else if (t == HEREDOC)
		return ("HEREDOC");
	else if (t == APPEND)
		return ("APPEND");
	else if (t == PIPE)
		return ("PIPE");
	return ("WTF??");
}

t_token	*get_tokens(char *line)
{
	char	**scmd;
	t_token	*tokens;

	g_state.duped_stdin = dup(0);
	line = spacify(line);
	if (!line)
		return (NULL);
	scmd = ft_split(line);
	free(line);
	if (!scmd)
		return (NULL);
	tokens = populate_tokens(scmd);
	if (!tokens)
		exit(1);
	if (syntax_error(tokens))
		return (destroy_tokens(tokens), write(2, "syntax error!\n", 14), \
					restore_stdin(), set_exit_code(NULL, 258), NULL);
	if (!expand(tokens))
		exit(1);
	tokens = wildcard(tokens);
	if (!tokens)
		exit(1);
	return (tokens);
}

int	get_last_exit_code(void)
{
	t_env	*e;

	e = g_state.g_env;
	while (e)
	{
		if (e->key_len == 1 && e->key[0] == '?')
			return (ft_atoi(e->value));
		e = e->next;
	}
	return (0);
}

static char	*_readline(void)
{
	char	*line;

	signal(SIGINT, handle_sigint_idle);
	line = readline(RL_PROMPT);
	if (!line)
	{
		write(1, "exit\n", 6);
		exit(get_last_exit_code());
	}
	if (!ft_strlen(line))
	{
		free(line);
		return (NULL);
	}
	return (line);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;

	rl_catch_signals = 0;
	if (argv && argc != 1)
		return (1);
	if (!build_env(envp))
		return (1);
	while (1)
	{
		line = _readline();
		if (!line)
			continue ;
		add_history(line);
		tokens = get_tokens(line);
		if (!tokens)
			continue ;
		cmd = build_command(tokens);
		if (!cmd)
			return (1);
		ft_excute_cmd(cmd);
		free_all(tokens, cmd);
	}
	return (0);
}
