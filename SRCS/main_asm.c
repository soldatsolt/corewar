#include "corewar.h"
#include "tab.h"
/*
** Начинаю делать проект корвар с асм
** Этот коммент больше для себя
**
*/

int			if_its_instr(char *str)
{
	int		len;
	int		i;

	i = 15;
	str = ft_strtrim(str);
	len = (int)ft_strlen(str);
	while (i >= 0)
	{
		if (len >= (int)ft_strlen(op_tab[i].name))
		{
			if (!ft_strncmp(str, op_tab[i].name, (int)ft_strlen(op_tab[i].name)))
			{
				if (str[(int)ft_strlen(op_tab[i].name)] != LABEL_CHAR)
				{
					free(str);
					return (i + 1);
				}
			}
		}
		i--;
	}
	free(str);
	return (0);
}

char		*last_instr_name(t_asm *a)
{
	t_token	*tmp;

	tmp = a->tokens;
	while(tmp->next)
	{
		tmp = tmp->next;
	}
	return (op_tab[tmp->instr - 1].name);
}

int			if_l_chars(char *str, t_asm *a)
{
	int		i;
	int		l;
	int		c;

	i = 0;
	c = ft_strchr_n(str, LABEL_CHAR);
	if (c < 0)
		return (0);
	str[c] = 0;
	l = (int)ft_strlen(str);
	while (i < l)
	{
		if (!ft_strchr(LABEL_CHARS, str[i]))
		{
			printf("NON LABEL CHARS IN LABEL ON LINE[%d]!!\n", a->current_line_number);
			return (0);
		}
		i++;
	}
	a->current_label = ft_strdup(str);
	str[c] = LABEL_CHAR;
	return (1);
}

char		*next_arg(char *str)
{
	int		c;
	int		n;
	char	*s;

	c = ft_strchr_n(str, SEPARATOR_CHAR);
	if (c < 0)
	{
		n = first_space_char(str);
		str[n] = 0;
		return (ft_strdup(str));
	}
	str[c] = 0;
	s = ft_strdup(str);
	str[c] = SEPARATOR_CHAR;
	return (s);
}

void		prep_for_next_arg(t_asm *a, char **str, int f, char **to_free)
{
	int		c;

	c = ft_strchr_n(*str, SEPARATOR_CHAR);
	if ((c < 0 && f != 1) || (c >= 0 && f == 1))
	{
		printf("=======SOMEtHING IS WRONG!!!!!!!!!!!!!===!\n"); //TODO: Нужно сделать нормальный выход
		free_parse_exit(a, 1, to_free);
		exit(1);
	}
	if (c < 0)
		return ;
	*str += c + 1;
	skip_whitespaces(str);
}

void		parse_args_instr(t_asm *a, char *str, char **to_free)
{
	char	*s;
	char	*last_inst;
	char	*last_arg;
	int		i;
	t_token	*curr_instr;

	curr_instr = last_instr(a);
	i = 0;
	last_inst = last_instr_name(a);
	s = ft_strstr(str, last_inst) + ft_strlen(last_inst);
	skip_whitespaces(&s);
	printf("---------------\n");
	printf("%s\n", s);
	while (i < op_tab[a->current_instruction - 1].n_args)
	{
		last_arg = next_arg(s);
		prep_for_next_arg(a, &s, op_tab[a->current_instruction - 1].n_args - i, to_free);
		printf("ARG = %s\n", last_arg);
		curr_instr->args[i] = last_arg;
		i++;
	}
	printf("---------------%s\n", s);
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
	if (str[0] && !if_its_instr(str) && if_l_chars(str, a))
		label_to_tokens(a, str);
	else if (if_its_instr(str))
	{
		a->current_instruction = if_its_instr(str);
		instr_to_tokens(a, str);
		parse_args_instr(a, str, &str);
	}
	free(str);
}

int			read_from_file(int fd, t_asm *a)
{
	char	*str;

	a->current_line_number = 1;
	while (get_next_line(fd, &str))
	{
		parse_this_line(fd, str, a);
		// if (str[ft_strlen(str) - 1] != '\n')
		// 	free_parse_exit(a, 1, &str);
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
	t_token	*tmp;

	a = init_asm();
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




	printf("_______________________\n_______________________\n");
	tmp = a.tokens;
	int i;
	while(tmp)
	{
		i = 0;
		if (tmp->type == 1)
		{
			printf("[ INSTR | %s", op_tab[tmp->instr - 1].name);
			while (i < op_tab[tmp->instr - 1].n_args)
			{
				printf(" ,%s ", tmp->args[i]);
				i++;
			}
			printf("]\n");
		}
		else if (tmp->type == 0)
			printf("[ LABEL | %s ]\n", tmp->label);
		tmp = tmp->next;
	}
	printf("[NULL]\n");
	free_parse_exit(&a, 0, 0);
	return (0);
}
