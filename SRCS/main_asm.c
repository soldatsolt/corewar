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

void		insert_all_between_quotes_to_str(int fd, char *str, int limit, t_asm *a)
{
	char	*to;
	char	*tmp_str;
	int		i;
	int		gnl;

	i = 0;
	gnl = 2;
	to = (limit == COMMENT_LENGTH) ? a->comment : a->name;
	tmp_str = ft_strchr(str, '"');
	tmp_str++;
	i = ft_strchr_n(tmp_str, '"');
	while (i < 0 && gnl > 0)
	{
		(gnl == 2) ? ft_strcpy(to, tmp_str) : ft_strcpy(to, str);
		to += (gnl == 2) ? (int)ft_strlen(to) : (int)ft_strlen(str);
		ft_strcpy(to, "\n");
		to++;
		if (gnl != 2)
			free(str);
		gnl = get_next_line(fd, &str);
		a->current_line_number++;
		i = ft_strchr_n(str, '"');
	}
	(gnl == 2) ? ft_strncpy(to, tmp_str, ft_strchr_n(tmp_str, '"')) : ft_strncpy(to, str, ft_strchr_n(str, '"'));
	(gnl == 2) ? i = 1 : free(str);
}

void		insert_comment_to_asm(int fd, char *str, t_asm *a)
{
	char	*tmp_str;
	int		i;
	int 	gnl;

	gnl = 1;
	i = 0;
	if (a->f & F_COMMENT)
	{
		ft_putendl_fd("Comment was declared before", 2);
		exit(1);
	}
	a->f |= F_COMMENT;
	// ft_printf("FLAG = %d\n", a->f);
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
	// ft_printf("FLAG = %d\n", a->f);
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
	{
		// ft_printf("NAME IS ON  %d\n", a->current_line_number);
		// ft_printf("STR IS  %p\n", &str);
		insert_name_to_asm(fd, str, a);
		// ft_printf("STR IS  %p\n", &str);
	}
	else if (!ft_strncmp(str, COMMENT_CMD_STRING, 8))
	{
		// ft_printf("COMMENT ON %d\n", a->current_line_number);
		insert_comment_to_asm(fd, str, a);
	}
	// else if (!ft_strncmp(str, "1111", 4))
	// {
	// 	ft_printf("ERROR ON LINE: %d\n", a->current_line_number);
	// }
	free(str);
}

int			read_from_file(int fd, t_asm *a)
{
	char	*str;

	a->current_line_number = 1;
	while (get_next_line(fd, &str))
	{
		// ft_printf("%s\n", str);
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
		printf("NAME IS |%s|\nCOMMENT IS |%s|\n", a.name, a.comment);
	}
	return (0);
}




