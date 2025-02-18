/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 14:54:45 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/11 14:56:13 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long	pl_m(const char *s, size_t j, int c)
{
	long long		p;

	p = 0;
	while (s[j] >= '0' && s[j] <= '9')
	{
		p = (p * 10) + (s[j] - 48);
		if (p > 9223372036854775807 && c == 1)
			return (-1);
		if (c == -1 && p > 9223372036854775807)
			return (0);
		j++;
	}
	return (p);
}

long long	ft_atoi(const char *str)
{
	size_t		i;
	size_t		c;
	long long	s;

	i = 0;
	c = 1;
	s = 0;
	while (str[i] == '\f' || str[i] == '\t' || str[i] == ' ' \
			|| str[i] == '\n' || str[i] == '\r' || str[i] == '\v')
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			c = -1;
		i++;
	}
	s = pl_m(str, i, c);
	return ((s * c));
}
