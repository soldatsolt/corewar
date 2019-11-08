/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_catstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/16 17:47:49 by kmills            #+#    #+#             */
/*   Updated: 2019/07/22 04:56:09 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_catstr(const char *s1, char *s2)
{
	char	*s;

	if (!s1 || *s1 == '\0')
		return (s2);
	s = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + 1);
	s = ft_strcpy(s, s1);
	s = ft_strcpy(s + ft_strlen(s1), s2);
	s -= ft_strlen(s1);
	s[ft_strlen(s1) + ft_strlen(s2)] = '\0';
	return (s);
}
