/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 18:19:31 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/11 16:56:58 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit(t_command *cmd, int pid)
{
	int	status;
	int	wpid;

	while (1)
	{
		wpid = waitpid(pid, &status, 0);
		if (wpid == pid)
		{
			if (WIFEXITED(status))
			{
				cmd->set_exit_code(cmd, WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == 2)
					cmd->set_exit_code(cmd, 130);
				else if (WTERMSIG(status) == 3)
					cmd->set_exit_code(cmd, 131);
				else if (WTERMSIG(status) == 11)
					cmd->set_exit_code(cmd, 139);
			}
			break ;
		}
	}
}

void	ft_excute_one_cmd(t_command *cmd, t_pipex ppx)
{
	ppx.pr = fork();
	if (ppx.pr < 0)
		perror("fork");
	else if (ppx.pr == 0)
	{
		if (ft_open_red(cmd, &ppx) < 0)
			exit (1);
		ft_dup_in_out(cmd, &ppx);
		ft_close_in_out(&ppx);
		ft_execve_cmd(cmd);
	}
	else
		handle_exit(cmd, ppx.pr);
}

int	ft_sin_her(t_command *cmd, t_pipex *ppx)
{
	if (ft_check_here_doc(cmd) == 1)
	{
		ppx->inf_here = ft_herdoc(cmd->heredoc);
		if (ppx->inf_here == -1)
		{
			if (g_state.is_sigint)
			{
				g_state.is_sigint = 0;
				cmd->set_exit_code(cmd, 1);
				if (cmd->next || cmd->prev)
					ft_close_pipe(ppx);
				return (1);
			}
			if (cmd->next || cmd->prev)
				ft_close_pipe(ppx);
			cmd->set_exit_code(cmd, 0);
			return (1);
		}
	}
	return (0);
}

void	ft_simple_cmd(t_command *cmd, t_pipex ppx)
{
	if (ft_sin_her(cmd, &ppx))
		return ;
	if (is_builtin(cmd->name))
	{
		if (ft_open_red(cmd, &ppx) < 0)
		{
			cmd->set_exit_code(cmd, 1);
			return ;
		}
		handle_builtin(cmd, ppx.outfile);
		ft_close_in_out(&ppx);
	}
	else
		ft_excute_one_cmd(cmd, ppx);
	ft_close_in_out(&ppx);
}

void	ft_excute_cmd(t_command *cmd)
{
	t_pipex		ppx;

	update_signals_props(cmd);
	ppx.infile = 0;
	ppx.inf_here = 0;
	ppx.outfile = 1;
	if (!cmd->next)
		ft_simple_cmd(cmd, ppx);
	else
	{
		ppx.nbrcmd = ft_nbrcmd(cmd);
		ppx.pipend = malloc(sizeof(int) * 2 * (ppx.nbrcmd - 1));
		if (!ppx.pipend)
		{
			perror("malloc");
			exit(1);
		}
		ppx.index = 0;
		ft_creat_pipe(&ppx);
		ft_pipe(cmd, &ppx);
		free(ppx.pipend);
	}
	while (wait(NULL) > 0)
		;
}
