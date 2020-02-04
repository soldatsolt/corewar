#include "corewar.h"

/*
** Начинаю делать проект корвар с асм
** Этот коммент больше для себя
** Проверка на \n в самом конце
FIXME: stir10,%-510,%0 НЕ ДОЛЖНА РАБОТАТЬ, НО У МЕНЯ РАБОТАЕТ
НУЖНЫ ПРОБЕЛЫ / ТАБЫ ПОСЛЕ САМОЙ ИНСТРУКЦИИ!!
*/

t_op    op_tab[17] =
{
	{"live", 1, {T_DIR}, 1, 10, "alive", 0, 0},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"and (and  r1, r2, r3   r1&r2 -> r3", 1, 0},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"or  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"xor (xor  r1, r2, r3   r1^r2 -> r3", 1, 0},
	{"zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 1},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 1},
	{"fork", 1, {T_DIR}, 12, 800, "fork", 0, 1},
	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 1},
	{"lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1},
	{"aff", 1, {T_REG}, 16, 2, "aff", 1, 0},
	{0, 0, {0}, 0, 0, 0, 0, 0}
};

char	tok_to_str[10][15] = {
						"LABEL",
						"INSTRUCTION",
						"REGISTER",
						"DIRECT",
						"INDIRECT",
						"SEPARATOR",
						"DIRECT_LABEL",
						"INDIRECT_LABEL",
						"NEXT_LINE",
						"ERR"
};

char	g_magic_header[4] = 
{0x00, 0xea, 0x83, 0xf3};

char	g_null[4] = 
{0x00, 0x00, 0x00, 0x00};

int		g_dir_size[17] = 
{0, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 4, 2, 2, 4};


// TODO: СДЕЛАТЬ ЭТУ ФУНКЦИЮ НОРМАЛЬНОЙ И ПРИДУМАЬБ, КАК ЕЁ НОРМАЛЬНО ЮЗАТЬ
//МОЖНО КАК ДЛЯ ПОДСЧЕТА ПАМЯТИ, И ЕЩЁ ДЛЯ ЧЕГО-НИБУДЬ))
t_type			define_arg_type(t_token *t, int n)
{
	char	*str;

	if (n < 3)
		str = t->args[n];
	else
		return (ERR);
	if (str[0] == '%' && str[1] && str[1] == ':')
		return (DIRECT_LABEL);
	else if (str[0] == '%')
		return (DIRECT);
	else if (str[0] == 'r')
		return (REGISTER);
	else if (str[0] >= '0' && str[0] <= '9')
		return (INDIRECT);
	else
		return (ERR);
}

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
		free_parse_exit(a, 1, to_free);
	}
	if (c < 0)
		return ;
	*str += c + 1;
	skip_whitespaces(str);
}

int			size_of_args(t_token *t) // для первого live : instr = 1, arg = %42
{
	int		i;
	int		size;

	size = 0;
	i = 0;
	while (i < op_tab[t->instr - 1].n_args)
	{
		if (define_arg_type(t, i) == REGISTER)
			size++;
		else if (define_arg_type(t, i) == INDIRECT)
			size += 2;
		else if (define_arg_type(t, i) == DIRECT || define_arg_type(t, i) == DIRECT_LABEL)
			size += g_dir_size[t->instr];
		i++;
	}
	if (op_tab[t->instr - 1].code_arg_type)
		size++;
	return (size);
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
	if (s[0] != ' ' && s[0] != '\t' && (s[0] == 'r'))
	{
		free_parse_exit(a, 1, to_free);
	}
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
	a->exec_code_size += size_of_args(curr_instr);
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

void		write_to_exec(t_asm *a)
{
	int		o;

	o = open("MYFILE", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	char *test;
	test = (char *)malloc(4);
	write(o, g_magic_header, 4);
	write(o, a->name, PROG_NAME_LENGTH);
	write(o, g_null, 4);
	// TODO: Здесь должна быть часть EXEC кода чампиона. Где взять - хз((((
	write(o, a->comment, COMMENT_LENGTH);




	free(test);
	close(0);
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




	write_to_exec(&a);





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
				printf(" ,%s-->%s ", tmp->args[i], tok_to_str[define_arg_type(tmp, i)]);
				i++;
			}
			printf("]\n");
		}
		else if (tmp->type == 0)
			printf("[ LABEL | %s ]\n", tmp->label);
		tmp = tmp->next;
	}
	printf("[NULL]\n");
	printf("SIZE OF EXEC_CODE IS [%x]\n", a.exec_code_size);
	free_parse_exit(&a, 0, 0);
	return (0);
}
