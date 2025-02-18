/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:41:39 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 20:00:00 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_inp_out(t_pipex *ppx, t_command *cmd)
{
	if (cmd->out)
		ppx->outfile = ft_redirection_out(cmd->out);
	if (cmd->in)
		ppx->infile = ft_redirection_in(cmd->in);
	if (ppx->infile == -1 || ppx->outfile == -1)
		exit (1);
	if (ppx->index == 0)
		ft_first_cmd(cmd, ppx);
	else if (ppx->index == ppx->nbrcmd - 1)
		ft_last_cmd(cmd, ppx);
	else
		ft_m_cmd(cmd, ppx);
	ft_close_in_out(ppx);
	ft_close_pipe(ppx);
}

void	ft_creat_pipe(t_pipex *ppx)
{
	int		l;

	l = 0;
	while (l < ppx->nbrcmd - 1)
	{
		if (pipe(ppx->pipend + 2 * l) < 0)
		{
			write(2, "pip error\n", 11);
			exit(1);
		}
		l++;
	}
}

void	ft_close_in_out(t_pipex *ppx)
{
	if (ppx->inf_here != 0)
		close(ppx->inf_here);
	if (ppx->infile != 0)
		close(ppx->infile);
	if (ppx->outfile != 1)
		close(ppx->outfile);
}

void	ft_excute_child(t_command *cmd, t_pipex *ppx)
{
	ft_inp_out(ppx, cmd);
	if (is_builtin(cmd->name))
	{
		handle_builtin(cmd, ppx->outfile);
		ft_close_in_out(ppx);
		exit (cmd->exit_code);
	}
	else
		ft_execve_cmd(cmd);
}

void	ft_pipe(t_command *cmd, t_pipex *ppx)
{
	t_command	*tmp;

	while (cmd)
	{
		if (ft_sin_her(cmd, ppx))
			return ;
		ppx->pr = fork();
		if (ppx->pr < 0)
			perror("fork");
		else if (ppx->pr == 0)
			ft_excute_child(cmd, ppx);
		ft_close_in_out(ppx);
		ppx->index++;
		tmp = cmd->prev;
		cmd = cmd->next;
	}
	ft_close_pipe(ppx);
	handle_exit(tmp, ppx->pr);
}
