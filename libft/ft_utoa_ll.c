/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_ll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/20 00:49:58 by kmills            #+#    #+#             */
/*   Updated: 2019/07/22 02:25:13 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*keks(int l, unsigned long long m, unsigned long long nl)
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

char		*ft_utoa_ll(unsigned long long n)
{
	unsigned long long	m;
	unsigned long long	nl;
	int					l;

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
