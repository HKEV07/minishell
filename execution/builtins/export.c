/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:57:12 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/09 19:43:44 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	keys_count(void)
{
	t_env		*e;
	size_t		c;

	e = g_state.g_env;
	c = 0;
	while (e)
	{
		if (!should_skip_env(e) && !e->skip)
			c++;
		e = e->next;
	}
	return (c);
}

static void	copy_all_keys(char **v)
{
	size_t		i;
	t_env		*e;

	e = g_state.g_env;
	i = 0;
	while (e)
	{
		if (!should_skip_env(e) && !e->skip)
			v[i++] = e->key;
		e = e->next;
	}
	v[i] = NULL;
}

void	sort_keys(char **keys, size_t size)
{
	size_t		i;
	size_t		j;
	char		*temp;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strncmp(keys[j], keys[j + 1], ft_strlen(keys[j])) > 0)
			{
				temp = keys[j];
				keys[j] = keys[j + 1];
				keys[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	sorted_print(char **v, int fd)
{
	size_t		i;
	t_env		*e;

	i = 0;
	while (v[i])
	{
		e = get_env_by_key(v[i], ft_strlen(v[i]));
		if (e)
		{
			write(fd, "declare -x ", 11);
			write(fd, e->key, e->key_len);
			if (e->value)
			{
				write(fd, "=\"", 2);
				write(fd, e->value, e->value_len);
				write(fd, "\"\n", 2);
			}
			else
				write(fd, "\n", 1);
		}
		i++;
	}
	free(v);
}

void	export(t_command *cmd, int fd)
{
	size_t		i;
	char		**v;
	int			code;

	code = 0;
	if (cmd->argv[1])
	{
		i = 1;
		while (cmd->argv[i])
			if (valid_request(cmd->argv[i++], &code))
				_export(cmd->argv[i - 1], &code);
	}
	else
	{
		i = keys_count();
		v = malloc(sizeof(char *) * (i + 1));
		if (!v)
			return (cmd->set_exit_code(cmd, 1));
		copy_all_keys(v);
		sort_keys(v, i);
		sorted_print(v, fd);
	}
	if (cmd->next || cmd->prev)
		exit(code);
	cmd->set_exit_code(cmd, code);
}
