/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 13:01:23 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/11 18:05:51 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_open_here_doc(char *del, int expand, int fd)
{
	char	*bufer;
	char	*old;

	while (1)
	{
		bufer = readline("> ");
		if (!bufer && g_state.is_sigint)
			return (free(bufer), -1);
		else if (!bufer)
			break ;
		old = bufer;
		bufer = ft_strjoin(old, "\n");
		free(old);
		if (is_del(del, bufer))
			break ;
		if (expand)
		{
			old = bufer;
			bufer = expand_str(old, 0);
			free(old);
		}
		write(fd, bufer, ft_strlen(bufer));
		free(bufer);
	}
	return (free(bufer), 0);
}

int	ft_herdoc(t_heredoc *here)
{
	char	*del;
	int		fd;

	while (here)
	{
		unlink("/tmp/temp_heredoc");
		fd = open("/tmp/temp_heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0777);
		del = ft_strjoin(here->terminator, "\n");
		if (ft_open_here_doc(del, here->expand, fd) == -1)
			return (close(fd), free(del), -1);
		free(del);
		close(fd);
		if (!here->next)
			return (ft_read_from_file());
		here = here->next;
	}
	return (STDIN_FILENO);
}

int	ft_redirection_out(t_out_rediretion *out_red)
{
	int		outfile;

	while (out_red)
	{
		if (out_red->ambiguous)
			return (-1);
		if (!out_red->append)
			outfile = open(out_red->filename, \
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
		else
			outfile = open(out_red->filename, \
			O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (!out_red->next)
			return (outfile);
		out_red = out_red->next;
	}
	return (STDOUT_FILENO);
}

int	ft_redirection_in(t_in_rediretion *in_red)
{
	int		infile;
	int		is;

	is = 0;
	while (in_red)
	{
		if (in_red->ambiguous)
			return (-1);
		infile = open(in_red->filename, O_RDONLY, 0777);
		is = 1;
		in_red = in_red->next;
	}
	if (is == 1)
		return (infile);
	return (STDIN_FILENO);
}

int	ft_check_here_doc(t_command *cmd)
{
	while (cmd)
	{
		if (cmd->heredoc)
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
