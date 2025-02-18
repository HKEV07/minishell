/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:51 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/11 16:42:31 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_idle(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	set_exit_code(NULL, 1);
}

void	handle_signint_exec(int signum)
{
	(void)signum;
	if (!access("/tmp/temp_heredoc", F_OK))
	{
		g_state.is_sigint = 1;
		close(0);
		unlink("/tmp/temp_heredoc");
		return ;
	}
	if (!g_state.special_sigint)
		write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	handle_sigquit(int signum)
{
	(void)signum;
}

char	*get_cmd_name(char *cmd_name)
{
	size_t	i;

	i = ft_strlen(cmd_name);
	if (!i)
		return (cmd_name);
	while (i > 0)
	{
		if (cmd_name[i] == '/')
		{
			if (cmd_name[i + 1])
				return (&cmd_name[i + 1]);
			else
				return (cmd_name);
		}
		i--;
	}
	return (cmd_name);
}

void	update_signals_props(t_command *cmd)
{
	char	*cmd_name;

	while (cmd)
	{
		if (!cmd->skip && cmd->name)
		{
			cmd_name = get_cmd_name(cmd->name);
			if (ft_strlen(cmd_name) == 9 && \
				!ft_strncmp(cmd_name, "minishell", 9))
				g_state.special_sigint = 1;
			else if (ft_strlen(cmd_name) == 4 && \
				!ft_strncmp(cmd_name, "bash", 4))
				g_state.special_sigint = 1;
		}
		cmd = cmd->next;
	}
	signal(SIGINT, handle_signint_exec);
}
