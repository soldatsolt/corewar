#include "corewar.h"

/*
** Начинаю делать проект корвар с асм
** Этот коммент больше для себя
** Проверка на \n в самом конце
FIXME: НЕ ОБРАБОТАЛ КОММЕНТАРИИ!!!! 
ld	%0,r11#sdasdas
ПИШЕТ
[ INSTR | ld ,%0-->DIRECT  ,r11#sdasdas-->REGISTER ]




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
		if (len >= (int)ft_strlen(op_tab[i].name)) // FIXME: ЗДЕСЬ МОЖНО ВОТКНУТЬ ВСЁ В ОДИН ИФ)))))))))
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
	if (ft_strchr(last_arg, COMMENT_CHAR))
		curr_instr->args[i - 1][ft_strchr_n(last_arg, COMMENT_CHAR)] = '\0';
	else if (ft_strchr(last_arg, ALT_COMMENT_CHAR)) //TODO: СДЕЛАТЬ КАК-НИБУДЬ НОРМАЛЬНО
		curr_instr->args[i - 1][ft_strchr_n(last_arg, ALT_COMMENT_CHAR)] = '\0';
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

void		write_exec_code_size_to_bin(t_asm *a, int o)
{
	char	*str;
	int		n;
	char	*tobin;
	int		i;

	i = 0;
	tobin = ft_strnew(4);
	str = ft_itoa_base_small_ll((uint64_t)a->exec_code_size, 16);
	n = 8 - (int)ft_strlen(str);
	while (n > 1)
	{
		tobin[i] = '\0';
		n -= 2;
		i++;
	}
	while (i < 4)
	{
		if (i == 0)
			tobin[i] = a->exec_code_size / 0x1000000;
		else if (i == 1)
			tobin[i] = a->exec_code_size / 0x10000;
		else if (i == 2)
			tobin[i] = a->exec_code_size / 0x100;
		else if (i == 3)
			tobin[i] = a->exec_code_size % 0x100;
		i++;
	}
	write(o, tobin, 4);
	free(tobin);
	free(str);
}

char		put_args_type_to_str(t_token *t)
{
	int		i;
	char	c;
	int		code;

	i = 0;
	c = 0;
	while (i < op_tab[t->instr - 1].n_args)
	{
		if (define_arg_type(t, i) == REGISTER)
			code = REG_CODE;
		else if (define_arg_type(t, i) == DIRECT || define_arg_type(t, i) == DIRECT_LABEL)
			code = DIR_CODE;
		else if (define_arg_type(t, i) == INDIRECT)
			code = IND_CODE;
		c |= code << (6 - 2 * i);
		i++;
	}
	return (c);
}

char		*put_dir_arg(t_asm *a, t_token *t, char *str, int *i)
{		

	*i += g_dir_size[t->instr];
	return (str);
}

char		*put_ind_arg(t_asm *a, t_token *t, char *str, int *i)
{

	*i += 2;
	return (str);
}

/*
**	put_ind_arg   
**	ОН НАЧИНАЕТ ИСКАТЬ МЕТКУ ВВЕРХ, А НЕ ВНИЗ((
**	TODO: Из этой ф-ии нужно сделать выход из всей
**	проги на случай, если нужной метки не будет
*/

char		*put_dir_label_arg(t_asm *a, t_token *t, char *str, int *i)
{
	t_token	*tmp_label;
	t_token	*tmp_instr;
	t_token	*tmp;
	int		diff;

	diff = 0;
	tmp_label = NULL;
	tmp_instr = NULL;
	tmp = a->tokens;


	while (tmp)
	{
		if (tmp->type == LABEL)
		{
			if (!(ft_strcmp(tmp->label, a->current_label))) // Имя метки совпадает с нужным нам
			{
				if (!tmp_instr) // Нужной инструкции ещё не было
				{
					printf("Found new label! (DIFF IS 0 NOW)\n");
					diff = 0;
					tmp_label = tmp;
				}
				else if (!tmp_label) // Нужная инструкция уже пройдена и такой метки не было
				{
					break ;
				}
				else // Нужная инструкция уже пройдена и такая метка была
				{
					printf("OOOPSSS DIR_LABEL\n");
				}
				printf("FOUND LABEL\n");
			}
		}
		if (tmp->type == INSTRUCTION)
		{
			if (tmp == t)
			{
				tmp_instr = tmp;
				if (tmp_label) // нужная метка была, тогда просто выходим c -знаком
				{
					diff = -diff;
					break ;
				}
				else // Идём дальше в поисках нужной метки
				{

				}
				printf("FOUND INSTR\n");
			}
		}
		if (tmp_label && tmp->type == INSTRUCTION)
		{
			printf("adding [%s]\n", op_tab[tmp->instr - 1].name);
			diff += size_of_args(tmp) + 1;
			printf("NOW DIFF IS [%d]\n", diff);
		}
		if (tmp_label && tmp_instr)
		{
			printf("ALL HERE\n");
			break ;
		}
		tmp = tmp->next;
	}
	printf("FINAL DIFF IS [%d]\n", diff);
	*i += g_dir_size[t->instr];
	return (str);
}

void		write_instr_to_bin(t_asm *a, t_token *t, int o)
{
	char	*str;
	int		i;
	int		ia;

	i = 0;
	ia = 0;
	str = ft_strnew(size_of_args(t) + 1);
	str[i] = op_tab[t->instr - 1].op_num;
	i++;
	if (op_tab[t->instr - 1].code_arg_type)
	{
		str[i] = put_args_type_to_str(t);
		i++;
	}
	while (i < size_of_args(t) + 1) // точно ли +1?? ведь код самой операции уже зашифрован
	{
		if (define_arg_type(t, ia) == REGISTER)
		{
			str[i] = ft_atoi(t->args[ia] + 1);
			i++;
		}
		else if (define_arg_type(t, ia) == DIRECT)
			str = put_dir_arg(a, t, str, &i);
		else if (define_arg_type(t, ia) == DIRECT_LABEL)
		{
			a->current_label = (t->args[ia]) + 2;
			str = put_dir_label_arg(a, t, str, &i);
		}
		else if (define_arg_type(t, ia) == INDIRECT)
			str = put_ind_arg(a, t, str, &i);
		// FIXME: НА САМОМ ДЕЛЕ ЗДЕСЬ НЕ НУЖНО I++, ЭТО ЧТОБЫ НЕ ВХОДИЛ В ИНФ ЦИКЛ
		ia++;
	}
	write(o, str, size_of_args(t) + 1);
	free(str);
}

void		write_to_exec(t_asm *a)
{
	int		o;
	t_token	*tmp;

	tmp = a->tokens;
	o = open("MYFILE", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	write(o, g_magic_header, 4);
	write(o, a->name, PROG_NAME_LENGTH);
	write(o, g_null, 4);
	// TODO: Здесь должна быть часть EXEC кода чeмпиона. Где взять - хз((((
	write_exec_code_size_to_bin(a, o);
	write(o, a->comment, COMMENT_LENGTH);
	write(o, g_null, 4);
	while (tmp)
	{
		if (tmp->type == INSTRUCTION)
			write_instr_to_bin(a, tmp, o);
		tmp = tmp->next;
	}

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
