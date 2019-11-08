/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 15:43:10 by kmills            #+#    #+#             */
/*   Updated: 2019/09/18 14:24:16 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "libft.h"

static	int	counter(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	return (i);
}

static int	merger(char **store, char *note)
{
	char *temporary;

	temporary = *store;
	*store = ft_strjoin(*store, note);
	free(temporary);
	return (0);
}

static int	updating(char **store)
{
	char *temporary;

	temporary = *store;
	*store = ft_strsub(*store, counter(*store) + 1,
			ft_strlen(*store) - counter(*store));
	free(temporary);
	return (0);
}

static	int	to_delete(char **arr)
{
	if (*arr[0] == '\0')
	{
		free(*arr);
		*arr = NULL;
		return (0);
	}
	return (1);
}

int			get_next_line(const int fd, char **line)
{
	static char	*store[10000];
	char		*note;
	int			ret;

	if (!line || fd < 0 ||
		!(note = (char *)malloc(BUFF_SIZE + 1)) || read(fd, note, 0))
		return (-1);
	if (!store[fd])
		store[fd] = ft_strnew(0);
	while ((ft_strchr(store[fd], '\n') == NULL) &&
		(ret = read(fd, note, BUFF_SIZE)) > 0)
	{
		note[ret] = '\0';
		merger(&store[fd], note);
	}
	*line = ft_strsub(store[fd], 0, counter(store[fd]));
	if (to_delete(&store[fd]) == 0)
	{
		free(note);
		return (0);
	}
	updating(&store[fd]);
	free(note);
	return (1);
}
