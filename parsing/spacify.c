/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spacify.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:56 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:36 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	do_rest(char *cmd, size_t *i, size_t *tokens_count)
{
	size_t	len;

	if (*i > 0 && cmd[*i - 1] != ' ')
		*tokens_count += 1;
	len = find_next_diff(cmd + *i, cmd[*i]);
	if (!len)
	{
		len = count_c_until_diff(cmd + *i, cmd[*i]);
		*i += len;
	}
	else
	{
		*i += len;
	}
	if (cmd[*i] && cmd[*i] != ' ')
		*tokens_count += 1;
}

static size_t	get_spaced_len(char *cmd)
{
	size_t	i;
	size_t	tokens_count;
	size_t	len;

	i = 0;
	tokens_count = 0;
	while (cmd[i])
	{
		if (cmd[i] && (cmd[i] == S_QUOTE || cmd[i] == D_QUOTE))
		{
			len = find_next(&cmd[i] + 1, cmd[i]);
			i += len;
		}
		else if (cmd[i] && char_in_str("<>|", cmd[i]))
		{
			do_rest(cmd, &i, &tokens_count);
			continue ;
		}
		i++;
	}
	return (i + tokens_count);
}

static void	do_loop(char *new_cmd, char *cmd, size_t *i, size_t *j)
{
	size_t	strip_count;

	if (*j > 0 && new_cmd[*j - 1] != ' ')
		new_cmd[(*j)++] = ' ';
	strip_count = find_next_diff(cmd + *i, cmd[*i]);
	if (!strip_count)
	{
		strip_count = count_c_until_diff(cmd + *i, cmd[*i]);
		*j += ft_strncpy(new_cmd + *j, cmd + *i, strip_count);
		*i += strip_count;
	}
	else
	{
		*j += ft_strncpy(new_cmd + *j, cmd + *i, strip_count);
		*i += strip_count;
	}
	if (cmd[*i] && cmd[*i] != ' ')
		new_cmd[(*j)++] = ' ';
}

static void	handle_qs(char *new_cmd, char *cmd, size_t *i, size_t *j)
{
	size_t	strip_count;

	strip_count = find_next(&cmd[*i] + 1, cmd[*i]);
	*j += ft_strncpy(new_cmd + *j, cmd + *i, strip_count + 1);
	*i += strip_count + 1;
}

char	*spacify(char *cmd)
{
	char	*new_cmd;
	size_t	i;
	size_t	j;

	if (check_q_error(cmd))
		return (write(2, "error in quoation.\n", 19),
			restore_stdin(), free(cmd), set_exit_code(NULL, 258), NULL);
	new_cmd = _calloc(get_spaced_len(cmd) + 1);
	if (!new_cmd)
		return (NULL);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (cmd[i] && (cmd[i] == S_QUOTE || cmd[i] == D_QUOTE))
			handle_qs(new_cmd, cmd, &i, &j);
		else if (cmd[i] && char_in_str("<>|", cmd[i]))
		{
			do_loop(new_cmd, cmd, &i, &j);
			continue ;
		}
		else
			new_cmd[j++] = cmd[i++];
	}
	return (free(cmd), new_cmd);
}
