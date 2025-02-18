/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 21:50:52 by ibenaait          #+#    #+#             */
/*   Updated: 2023/07/29 21:50:52 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_msg(char *key, int *code)
{
	write(2, "minishell: unset: `", 19);
	write(2, key, ft_strlen(key));
	write(2, "': not a valid identifier\n", 26);
	*code = 1;
}

int	valid_key(char *key)
{
	size_t	i;

	if (char_in_str(NUMS, key[0]))
		return (0);
	i = 0;
	while (key[i])
	{
		if (!char_in_str(ENV_KEY_CHARS, key[i]))
			return (0);
		i++;
	}
	return (1);
}

void	unset(t_command *cmd)
{
	size_t		i;
	int			code;

	code = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (valid_key(cmd->argv[i]))
		{
			delete_env_by_key(cmd->argv[i], ft_strlen(cmd->argv[i]));
		}
		else
			print_error_msg(cmd->argv[i], &code);
		i++;
	}
	cmd->set_exit_code(cmd, code);
}
