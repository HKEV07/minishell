/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:36:32 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:33 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_new_argv(char *cmd_name)
{
	char	**av;

	av = malloc(sizeof(char *) * (2));
	if (!av)
		return (NULL);
	av[0] = cmd_name;
	av[1] = NULL;
	return (av);
}

char	**copy_old_argv(char **ov, size_t *count)
{
	char	**p;
	size_t	i;

	i = 0;
	while (ov[i])
		i++;
	p = malloc(sizeof(char *) * (*count + i + 1));
	if (!p)
		return (NULL);
	*count = i;
	i = 0;
	while (ov[i])
	{
		p[i] = ov[i];
		i++;
	}
	free(ov);
	return (p);
}

static void	set_ambg(void *r, t_token_type type, int ambg)
{
	if (type == R_IN)
		((t_in_rediretion *)r)->ambiguous = ambg;
	else
		((t_out_rediretion *)r)->ambiguous = ambg;
}

void	*add_redir(t_command *cmd, t_token *token)
{
	void			**head;
	void			*r;
	t_token_type	type;

	type = token->prev->type;
	if (type == R_IN)
		head = (void **)(&cmd->in);
	else
		head = (void **)(&cmd->out);
	r = redir_pushback(head, type);
	if (!r)
		return (NULL);
	if (type == R_IN)
		((t_in_rediretion *)r)->filename = remove_quotes(token->token);
	else
	{
		((t_out_rediretion *)r)->filename = remove_quotes(token->token);
		if (type == APPEND)
			((t_out_rediretion *)r)->append = 1;
		else
			((t_out_rediretion *)r)->append = 0;
	}
	set_ambg(r, type, token->prev->ambiguous);
	cmd->skip = token->prev->ambiguous;
	return (r);
}

int	create_new_cmd(t_command **cmd, t_command **cmd_head)
{
	*cmd = command_pushback(cmd_head);
	if (!*cmd)
		return (0);
	return (1);
}
