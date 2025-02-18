/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:49:07 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:49:08 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_number(char c)
{
	size_t		i;

	i = 0;
	while (NUMS[i])
	{
		if (NUMS[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	is_illegal_char(char *k, size_t i, int *append)
{
	if (is_number(k[i]) && i == 0)
		return (1);
	if (k[i] == '+')
	{
		if (i == 0)
			return (1);
		else if (i > 0 && k[i + 1])
			return (1);
		else
		{
			k[i] = '\0';
			*append = 1;
			return (0);
		}
	}
	if (!char_in_str(ENV_KEY_CHARS, k[i]))
		return (1);
	return (0);
}

int	invalid_key(char *k, char *e, int *code, int *append)
{
	size_t	i;

	i = 0;
	while (k[i])
	{
		if (is_illegal_char(k, i, append))
		{
			write(2, "minishell: export: `", 20);
			write(2, e, ft_strlen(e));
			write(2, "': not a valid identifier\n", 26);
			free(k);
			*code = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	valid_request(char *e, int *code)
{
	int		error;
	size_t	idx;

	error = 0;
	idx = get_index_of(e, '+');
	if (e[0] == '=')
		error = 1;
	else if ((!idx && e[0] == '+') || (idx > 0 && e[idx + 1] != '='))
		error = 1;
	if (error)
	{
		write(2, "minishell: export: `", 20);
		write(2, e, ft_strlen(e));
		write(2, "': not a valid identifier\n", 26);
		*code = 1;
		return (0);
	}
	return (1);
}
