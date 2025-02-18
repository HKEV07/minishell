/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:37:22 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:24 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_quotes(char *str, size_t pos)
{
	size_t	i;
	char	q;

	i = 0;
	q = 0;
	while (str[i])
	{
		if (!q && char_in_str("\"'", str[i]))
			q = str[i];
		else if (q && str[i] == q)
			q = 0;
		if (pos == i)
			return (q);
		i++;
	}
	return (0);
}
