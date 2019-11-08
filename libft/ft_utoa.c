/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 05:11:44 by kmills            #+#    #+#             */
/*   Updated: 2019/07/22 02:24:59 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*keks(int l, long m, long nl)
{
	char	*str;
	int		i;

	i = 0;
	if (!(str = (char *)ft_memalloc(sizeof(char) * (l + 2))))
		return (NULL);
	while (m > 0)
	{
		str[i++] = (nl / m + '0');
		nl = nl % m;
		m = m / 10;
	}
	str[i] = '\0';
	return (str);
}

char		*ft_utoa(unsigned int n)
{
	unsigned long	m;
	unsigned long	nl;
	int				l;

	m = 1;
	nl = n;
	l = 0;
	while (((m * 10) > 0) ? (nl / (m * 10) > 0) : 0)
	{
		m = m * 10;
		l++;
	}
	return (keks(l, m, nl));
}
