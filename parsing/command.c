/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:36:34 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:35 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*get_last_command(t_command *head)
{
	while (head)
	{
		if (head->next == NULL)
			return (head);
		head = head->next;
	}
	return (head);
}

static t_command	*new_command(t_command *prev, t_command *next)
{
	t_command	*cmd;

	cmd = _calloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->prev = prev;
	cmd->get_envp = get_envp;
	cmd->set_exit_code = set_exit_code;
	cmd->next = next;
	return (cmd);
}

t_command	*command_pushback(t_command **head)
{
	t_command	*c;
	t_command	*l;

	if (!*head)
	{
		c = new_command(NULL, NULL);
		if (!c)
			return (NULL);
		*head = c;
	}
	else
	{
		l = get_last_command(*head);
		c = new_command(l, NULL);
		if (!c)
			return (NULL);
		l->next = c;
	}
	return (c);
}

static void	get_argv_count(t_token *t, size_t *count)
{
	*count = 0;
	while (t)
	{
		if (t->type != WORD)
			break ;
		if (ft_strlen(t->token))
			(*count)++;
		t = t->next;
	}
}

char	**add_argv(t_command *cmd, t_token **token)
{
	size_t	count;
	char	**argv;

	get_argv_count(*token, &count);
	if (cmd->argv)
		argv = copy_old_argv(cmd->argv, &count);
	else
	{
		argv = malloc(sizeof(char *) * (count + 1));
		count = 0;
	}
	if (!argv)
		return (NULL);
	while (*token)
	{
		if ((*token)->type != WORD)
			break ;
		if (ft_strlen((*token)->token))
			argv[count++] = remove_quotes((*token)->token);
		cmd_check_and_move(cmd, token);
	}
	argv[count] = NULL;
	cmd->argv = argv;
	return (argv);
}
