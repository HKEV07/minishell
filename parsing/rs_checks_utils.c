/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rs_checks_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:31 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:32 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	rin_check(char *filename)
{
	struct stat	fstat;

	if (stat(filename, &fstat))
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": No such file or directory\n", 28);
		return (1);
	}
	if (access(filename, R_OK))
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": Permission denied\n", 20);
		return (1);
	}
	return (0);
}

int	rout_check(char *filename)
{
	struct stat	path_stat;

	if (stat(filename, &path_stat))
		return (0);
	if (S_ISDIR(path_stat.st_mode))
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": Is a directory\n", 17);
		return (1);
	}
	else if (access(filename, W_OK))
	{
		write(2, "minishell: ", 11);
		write(2, filename, ft_strlen(filename));
		write(2, ": Permission denied\n", 20);
		return (1);
	}
	return (0);
}

int	is_redir(t_token_type type)
{
	if (type == R_IN || type == R_OUT || type == APPEND)
		return (1);
	return (0);
}

void	delete_redir(t_token **t)
{
	t_token	*redir;

	redir = *t;
	*t = redir->next->next;
	if (redir->prev)
		redir->prev->next = redir->next->next;
	if (redir->next->next)
		redir->next->next->prev = redir->prev;
	free(redir->token);
	free(redir->next->token);
	free(redir->next);
	free(redir);
}

void	remove_all_following_rs(t_token **t, t_token *start)
{
	*t = start->next->next;
	while (*t)
	{
		if ((*t)->type == PIPE)
			return ;
		if (is_redir((*t)->type))
		{
			delete_redir(t);
			continue ;
		}
		*t = (*t)->next;
	}
}
