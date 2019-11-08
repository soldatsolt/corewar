/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 06:47:35 by kmills            #+#    #+#             */
/*   Updated: 2019/09/15 21:07:28 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*str;
	int		i;
	char	cc;

	if (!s)
		return (NULL);
	cc = (char)c;
	str = (char *)s;
	i = 1;
	while (*str && *str != cc)
	{
		str++;
		s++;
		i++;
	}
	if (*str == cc)
		return ((char *)s);
	return (NULL);
}
