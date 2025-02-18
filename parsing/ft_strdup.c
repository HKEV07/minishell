/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 14:55:29 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:36:51 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *s1)
{
	size_t	strlen;
	size_t	i;
	char	*sp;

	strlen = ft_strlen(s1);
	sp = malloc(strlen + 1);
	if (!sp)
		return (sp);
	i = 0;
	while (s1[i])
	{
		sp[i] = s1[i];
		i++;
	}
	sp[i] = '\0';
	return (sp);
}
