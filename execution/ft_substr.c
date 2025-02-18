/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 15:39:16 by ibenaait          #+#    #+#             */
/*   Updated: 2023/07/18 16:19:23 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	getlen(size_t slen, size_t start, size_t len)
{
	size_t	flen;

	flen = 0;
	while (flen + start < slen && flen < len)
		flen++;
	return (flen);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;
	size_t	flen;

	if (s == 0)
		return (0);
	i = ft_strlen(s);
	flen = getlen(i, start, len);
	str = (char *)malloc(sizeof(char) * (flen + 1));
	if (!str)
		return (0);
	ft_memmove(str, (char *)(s + start), flen);
	str[flen] = '\0';
	return (str);
}
