/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:51:49 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:14 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cpystr(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sp;
	size_t	strlen;

	if (!s)
		return (0);
	strlen = ft_strlen(s);
	if (strlen <= start)
	{
		sp = malloc(1);
		if (!sp)
			return (0);
		*sp = '\0';
	}
	else
	{
		if ((strlen - start) < len)
			len = strlen - start;
		sp = malloc(len + 1);
		if (!sp)
			return (0);
		cpystr(sp, s + start, len);
	}
	return (sp);
}
