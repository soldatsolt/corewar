#include "corewar.h"

/*
** Начинаю делать проект корвар с асм
** Этот коммент больше для себя
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
*/

void		u_lose(int need_free, char *to_free, char *note, t_asm *a)
{
	if (need_free == 1)
	{
		ft_printf("%s on line %i\n", note, a->current_line_number);
		free(to_free);
		exit(1);
	}
	else
	{
		ft_printf("%s on line %i\n", note, a->current_line_number);
		exit(1);
	}
	return ;
}

void		insert_all_between_quotes_to_str(int fd, char *str, int limit, t_asm *a)
{
	char	*to;
	char	*tmp_str;
	int		i;
	int		gnl;
	int		wordlen;

	wordlen = 0;
	i = 0;
	gnl = 2;
	to = (limit == COMMENT_LENGTH) ? a->comment : a->name;
	tmp_str = ft_strchr(str, '"');
	if (!tmp_str)
		return(u_lose(0, str, "No name was detected", a));
	tmp_str++;
	i = ft_strchr_n(tmp_str, '"');
	while (i < 0 && gnl > 0)
	{
		if (wordlen + ((gnl == 2) ? (int)ft_strlen(tmp_str) : (int)ft_strlen(str)) > limit + 1)
			return (u_lose((gnl == 2) ? 0 : 1, str, (limit == COMMENT_LENGTH) ? CCTL : CNTL, a));
		(gnl == 2) ? ft_strcpy(to + wordlen, tmp_str) : ft_strcpy(to + wordlen, str);
		wordlen += (gnl == 2) ? (int)ft_strlen(to) : (int)ft_strlen(str);
		ft_strcpy(to + wordlen++, "\n");
		(gnl != 2) ? free(str) : i;
		gnl = get_next_line(fd, &str);
		a->current_line_number++;
		i = ft_strchr_n(str, '"');
	}
	if (wordlen + ((gnl == 2) ? (int)ft_strlen(tmp_str) : (int)ft_strlen(str)) > limit + 1)
		return (u_lose((gnl == 2) ? 0 : 1, str, (limit == COMMENT_LENGTH) ? CCTL : CNTL, a));
	(gnl == 2) ? ft_strncpy(to + wordlen, tmp_str, ft_strchr_n(tmp_str, '"')) : ft_strncpy(to + wordlen, str, ft_strchr_n(str, '"'));
	(gnl == 2) ? i = 1 : free(str);
}

void		insert_comment_to_asm(int fd, char *str, t_asm *a)
{
	char	*tmp_str;
	int		i;
	int		gnl;

	gnl = 1;
	i = 0;
	if (a->f & F_COMMENT)
	{
		ft_putendl_fd("Comment was declared before", 2);
		exit(1);
	}
	a->f |= F_COMMENT;
	ft_bzero(a->comment, COMMENT_LENGTH);
	insert_all_between_quotes_to_str(fd, str, COMMENT_LENGTH, a);
}

void		insert_name_to_asm(int fd, char *str, t_asm *a)
{
	char	*tmp_str;
	int		i;

	i = 0;
	if (a->f & F_NAME)
	{
		ft_putendl_fd("Name was declared before", 2);
		exit(1);
	}
	a->f |= F_NAME;
	ft_bzero(a->name, PROG_NAME_LENGTH);
	insert_all_between_quotes_to_str(fd, str, PROG_NAME_LENGTH, a);
}

int			first_non_space_char(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (i);
}

void		parse_this_line(int fd, char *str, t_asm *a)
{
	str = ft_strtrim(str);
	if (!str[0])
	{
		free(str);
		return ;
	}
	if (!ft_strncmp(str, NAME_CMD_STRING, 5))
		insert_name_to_asm(fd, str, a);
	else if (!ft_strncmp(str, COMMENT_CMD_STRING, 8))
		insert_comment_to_asm(fd, str, a);
	free(str);
}

int			read_from_file(int fd, t_asm *a)
{
	char	*str;

	a->current_line_number = 1;
	while (get_next_line(fd, &str))
	{
		parse_this_line(fd, str, a);
		free(str);
		str = NULL;
		a->current_line_number++;
	}
	if (str)
		free(str);
	return (1);
}

int			main(int argc, char **argv)
{
	int		fd;
	char	str[BUFF_SIZE + 1];
	t_asm	a;

	a.f = 0;
	if (argc < 2)
		ft_putendl("Usage: ./vm_champs/asm <sourcefile.s>");
	else
	{
		fd = open(argv[argc - 1], O_RDONLY);
		if (read(fd, str, 0) == -1)
		{
			perror("Can't read a file");
			return (1);
		}
		read_from_file(fd, &a);
		close(fd);
		printf("NAME IS |%s|\nCOMM IS |%s|\n", a.name, a.comment);
	}
	return (0);
}
