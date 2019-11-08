/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 20:10:53 by kmills            #+#    #+#             */
/*   Updated: 2019/09/05 22:30:16 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*spcs(char const *s, char c)
{
	char	*str;
	int		i;
	int		l;

	i = -1;
	if (s == NULL)
		return (NULL);
	l = ft_strlen((char *)s);
	while (s[l - 1] == c)
		l--;
	while (s[++i] == c)
		l--;
	if (l <= 0)
		l = 0;
	if (l == 0 || !(str = ft_strnew((size_t)l)))
		return (NULL);
	s = s + i;
	i = -1;
	while (++i < l)
		str[i] = *s++;
	str[i] = '\0';
	return (str);
}

static int	*lengses(int *lengs, char *s, char c)
{
	int	l;
	int	i;
	int n;

	i = 0;
	l = 0;
	n = 0;
	while (s[i])
	{
		while (s[i] && (s[i] != c))
		{
			i++;
			l++;
		}
		lengs[n] = l;
		n++;
		l = 0;
		while (s[i] && (s[i] == c))
			i++;
	}
	return (lengs);
}

static int	count_words(char *s, char c)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (s[i])
	{
		while (s[i] && (s[i] != c))
			i++;
		n++;
		while (s[i] && (s[i] == c))
			i++;
	}
	return (n);
}

static char	**fillstr(char **str, char *s, char c)
{
	int x;
	int i;
	int y;

	i = 0;
	x = 0;
	y = 0;
	while (s[i])
	{
		while (s[i] && (s[i] != c))
		{
			str[y][x] = s[i];
			i++;
			x++;
		}
		str[y][x] = '\0';
		x = 0;
		y++;
		while (s[i] && (s[i] == c))
			i++;
	}
	str[y] = NULL;
	return (str);
}

char		**ft_strsplit(char const *ss, char c)
{
	char	*s;
	char	**str;
	int		i;
	int		*lengs;

	if (ss == NULL)
		return (NULL);
	i = 0;
	s = (char *)ss;
	s = spcs(s, c);
	if (!ft_strlen(s))
		return (NULL);
	str = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	lengs = (int *)malloc(sizeof(int) * count_words(s, c));
	lengs = lengses(lengs, s, c);
	while (i < count_words(s, c))
	{
		str[i] = (char *)malloc(sizeof(char) * (lengs[i] + 2));
		i++;
	}
	str = fillstr(str, s, c);
	free(lengs);
	free(s);
	return (str);
}
