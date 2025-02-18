/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbouhach <hbouhach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:52:02 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/10 22:37:46 by hbouhach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strncpy(char *dst, char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	return (i);
}

void	*_calloc(size_t __size)
{
	void	*ptr;
	size_t	i;

	ptr = malloc(__size);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < __size)
		((char *)ptr)[i++] = '\0';
	return (ptr);
}

void	set_exit_code(t_command *self, int exit_code)
{
	t_env	*e;

	if (self)
		self->exit_code = exit_code;
	e = get_env_by_key("?", 1);
	if (e)
	{
		free(e->value);
		e->value = ft_itoa(exit_code);
		e->value_len = ft_strlen(e->value);
	}
}
