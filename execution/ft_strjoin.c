/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 22:14:55 by ibenaait          #+#    #+#             */
/*   Updated: 2023/08/11 18:06:01 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	str = NULL;
	if (!s1)
	{
		s1 = malloc(1);
		if (!s1)
			return (0);
	}
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (0);
	while (s1[i])
		str[j++] = s1[i++];
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = 0;
	return (str);
}

int	ft_nbrcmd(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

int	is_del(char *del, char *bufer)
{
	if ((ft_strlen(del) == ft_strlen(bufer)) && \
			!ft_strncmp(del, bufer, ft_strlen(del)))
		return (1);
	return (0);
}
