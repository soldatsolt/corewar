/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 05:52:02 by kmills            #+#    #+#             */
/*   Updated: 2019/09/17 20:36:36 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*s;

	if (size + 1 < size)
		return (NULL);
	if (!(s = (char *)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	if (size)
		ft_bzero((void *)s, size);
	s[size] = '\0';
	return (s);
}
