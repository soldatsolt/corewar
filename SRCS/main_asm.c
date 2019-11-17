#include "corewar.h"
#include "op.h"

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

void		insert_comment_to_asm(char *str, t_asm *a)
{
	char	*tmp_str;
	int		i;

	i = 0;
	tmp_str = ft_strchr(str, '"');
	tmp_str++;
	i = ft_strchr_n(tmp_str, '"');
	a->comment = ft_strsub(tmp_str, 0, i);
}

void		insert_name_to_asm(char *str, t_asm *a)
{
	char	*tmp_str;
	int		i;

	i = 0;
	tmp_str = ft_strchr(str, '"');
	tmp_str++;
	i = ft_strchr_n(tmp_str, '"');
	a->name = ft_strsub(tmp_str, 0, i);
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

void		parse_this_line(char *str, int i, t_asm *a)
{
	str = ft_strtrim(str);
	if (!str[0])
	{
		free(str);
		return ;
	}
	if (!ft_strncmp(str, NAME_CMD_STRING, 5))
	{
		ft_printf("NAME IS ON  %d\n", i);
		insert_name_to_asm(str, a);
	}
	if (!ft_strncmp(str, COMMENT_CMD_STRING, 8))
	{
		ft_printf("COMMENT ON %d\n", i);
		insert_comment_to_asm(str, a);
		// exit(1);
	}

	free(str);
}

int			read_from_file(int fd, t_asm *a)
{
	int		i;
	char	*str;

	i = 1;
	while (get_next_line(fd, &str))
	{
		ft_printf("%s\n", str);
		parse_this_line(str, i, a);
		free(str);
		str = NULL;
		i++;
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




