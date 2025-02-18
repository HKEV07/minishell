/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utlis_fun_one.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 21:16:06 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/08 22:23:12 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_mes_one(char *name, char *str)
{
	write(2, "minishell: ", 11);
	write(2, name, ft_strlen(name));
	write(2, str, ft_strlen(str));
}

void	ft_duplicate(int out, int in)
{
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
}

void	ft_close_pipe(t_pipex *ppx)
{
	int		i;

	i = 0;
	while (i < 2 * (ppx->nbrcmd - 1))
	{
		close(ppx->pipend[i]);
		i++;
	}
}

int	is_not_cmd(char *cmd)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (0);
		i++;
	}
	return (1);
}

int	ft_open_red(t_command *cmd, t_pipex *ppx)
{
	if (cmd->in)
		ppx->infile = ft_redirection_in(cmd->in);
	if (cmd->out)
		ppx->outfile = ft_redirection_out(cmd->out);
	if ((ppx->infile == -1 || ppx->outfile == -1))
		return (-1);
	return (0);
}
