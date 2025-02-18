/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utlis_dup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:39:17 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 22:22:04 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_first_cmd(t_command *cmd, t_pipex *ppx)
{
	if (cmd->out && (cmd->in && cmd->last_redir == R_IN))
		ft_duplicate(ppx->outfile, ppx->infile);
	else if (cmd->out && (cmd->heredoc && cmd->last_redir == HEREDOC))
		ft_duplicate(ppx->outfile, ppx->inf_here);
	else if (cmd->out)
		dup2(ppx->outfile, STDOUT_FILENO);
	else if (cmd->in && cmd->last_redir == R_IN)
		ft_duplicate(ppx->pipend[1], ppx->infile);
	else if (cmd->heredoc && cmd->last_redir == HEREDOC)
		ft_duplicate(ppx->pipend[1], ppx->inf_here);
	else
		dup2(ppx->pipend[1], STDOUT_FILENO);
}

void	ft_m_cmd(t_command *cmd, t_pipex *ppx)
{
	if (cmd->out && (cmd->in && cmd->last_redir == R_IN))
		ft_duplicate(ppx->outfile, ppx->infile);
	else if (cmd->out && (cmd->heredoc && cmd->last_redir == HEREDOC))
		ft_duplicate(ppx->outfile, ppx->inf_here);
	else if (cmd->out)
		ft_duplicate(ppx->outfile, ppx->pipend[2 * ppx->index - 2]);
	else if (cmd->in && cmd->last_redir == R_IN)
		ft_duplicate(ppx->pipend[2 * ppx->index + 1], ppx->infile);
	else if (cmd->heredoc && cmd->last_redir == HEREDOC)
		ft_duplicate(ppx->pipend[2 * ppx->index + 1], ppx->inf_here);
	else
		ft_duplicate(ppx->pipend[2 * ppx->index + 1], \
		ppx->pipend[2 * ppx->index - 2]);
}

void	ft_last_cmd(t_command *cmd, t_pipex *ppx)
{
	if (cmd->out && (cmd->in && cmd->last_redir == R_IN))
		ft_duplicate(ppx->outfile, ppx->infile);
	else if (cmd->out && (cmd->heredoc && cmd->last_redir == HEREDOC))
		ft_duplicate(ppx->outfile, ppx->inf_here);
	else if (cmd->out)
		ft_duplicate(ppx->outfile, ppx->pipend[2 * ppx->index - 2]);
	else if (cmd->in && cmd->last_redir == R_IN)
		dup2(ppx->infile, STDIN_FILENO);
	else if (cmd->heredoc && cmd->last_redir == HEREDOC)
		dup2(ppx->inf_here, STDIN_FILENO);
	else 
		dup2(ppx->pipend[2 * ppx->index - 2], STDIN_FILENO);
}

int	ft_read_from_file(void)
{
	int		fd;

	fd = open("/tmp/temp_heredoc", O_RDONLY, 0777);
	return (fd);
}

void	ft_dup_in_out(t_command *cmd, t_pipex *ppx)
{
	if (cmd->out && (cmd->in && cmd->last_redir == R_IN))
		ft_duplicate(ppx->outfile, ppx->infile);
	else if (cmd->out && (cmd->heredoc && cmd->last_redir == HEREDOC))
		ft_duplicate(ppx->outfile, ppx->inf_here);
	else if (cmd->out)
		dup2(ppx->outfile, STDOUT_FILENO);
	else if (cmd->in && cmd->last_redir == R_IN)
		dup2(ppx->infile, STDIN_FILENO);
	else if (cmd->heredoc && cmd->last_redir == HEREDOC)
		dup2(ppx->inf_here, STDIN_FILENO);
}
