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

void		skip_whitespaces(char	**str)
{
	while(**str == ' ' || **str == '\t')
		(*str)++;
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
	int		i;
	int		gnl;

	gnl = 1;
	i = 0;
	if (a->f & F_COMMENT)
	{
		ft_printf("SYNTAX ERROR ON LINE [%d]\n", a->current_line_number);
		ft_putendl_fd("Comment was declared before", 2);
		exit(1);
	}
	a->f |= F_COMMENT;
	ft_bzero(a->comment, COMMENT_LENGTH);
	insert_all_between_quotes_to_str(fd, str, COMMENT_LENGTH, a);
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
					// printf("THE INSTR IS [%s]\n", str);
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

void		insert_name_to_asm(int fd, char *str, t_asm *a)
{
	int		i;

	i = 0;
	if (a->f & F_NAME)
	{
		ft_printf("SYNTAX ERROR ON LINE [%d]\n", a->current_line_number);
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

t_token		*last_instr(t_asm *a)
{
	t_token	*tmp;

	tmp = a->tokens;
	while(tmp->next)
	{
		tmp = tmp->next;
	}
	return (tmp);
}

int			first_space_char(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i] && (str[i] != ' ' && str[i] != '\t'))
		i++;
	return (i);
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

void		free_token_content(t_token *t)
{
	int		i;

	i = 0;
	if (t->type == INSTRUCTION)
	{
		while (i < 3 && t->args[i])
		{
			free(t->args[i]);
			i++;
		}
	}
	else if (t->type == LABEL)
	{
		if (t->label)
		{
			free(t->label);
			t->label = NULL;
		}
	}
}

void		free_all_tokens(t_token *t)
{
	t_token	*tmp;

	while (t)
	{
		tmp = t;
		t = t->next;
		free_token_content(tmp);
		free(tmp);
	}
}

void		free_parse_exit(t_asm *a, int er_flag, char **to_free)
{
	free_all_tokens(a->tokens);
	if (er_flag)
		ft_printf("LEX ERROR ON LINE [%d]\n", a->current_line_number);
	if (to_free)
		free(*to_free);
	exit(1);
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

t_token		*init_instruction(t_token *t, char *str)
{
	t = (t_token *)malloc(sizeof(t_token));
	t->type = INSTRUCTION;
	t->instr = if_its_instr(str);
	t->next = NULL;
	t->args[0] = NULL;
	t->args[1] = NULL;
	t->args[2] = NULL;
	t->label = NULL;
	return (t);
}

void		instr_to_tokens(t_asm *a, char *str)
{
	t_token		*tmp_token;

	tmp_token = a->tokens;
	if(a->tokens)
	{
		while (tmp_token->next)
			tmp_token = tmp_token->next;
		tmp_token->next = init_instruction(tmp_token->next, str);
		// tmp_token->next = (t_token *)malloc(sizeof(t_token));
		// tmp_token->next->type = INSTRUCTION;
		// tmp_token->next->instr = if_its_instr(str);
		// tmp_token->next->next = NULL;
		// tmp_token->next->args[0] = 0;
		// printf("[%s] TOKEN TYPE IS [INSTRUCTION] (%d), %d\n", str, tmp_token->next->type, tmp_token->next->instr);
	}
	else
	{
		a->tokens = init_instruction(a->tokens, str);
		// a->tokens = (t_token *)malloc(sizeof(t_token));
		// a->tokens->type = INSTRUCTION;
		// a->tokens->instr = if_its_instr(str);
		// a->tokens->next = NULL;

		// printf("[%s] TOKEN TYPE IS [INSTRUCTION] (%d), %d\n", str, a->tokens->type, a->tokens->instr);
	}


}

void		skip_label_if_instr_ferther(t_asm *a, char *str)
{
	// эта фунция режет начало метки на случай, если сразу за ней (на этой же строчке)
	// будет идти инструкция
	void	*to_free;
	// FIXME: ЕСЛИ СДЕЛАТЬ TO_FREE CHAR*, ТО ОН БУДЕТ ПОЛЗТИ ЗА СТР
	// FIXME: ПОЧЕМУ????? FIXME: FIXME: ??????????????????????/
	to_free = (void *)str;
	while (ft_strchr(LABEL_CHARS, *str))
		str++;
	str++;
	while (*str == '\t' || *str == ' ')
		str++;
	if (if_its_instr(str))
	{
		a->current_instruction = if_its_instr(str);
		instr_to_tokens(a, str);
		parse_args_instr(a, str, (char **)&to_free);
	}
}

t_token		*init_label(t_asm *a, t_token *t, char *str)
{
	t = (t_token *)malloc(sizeof(t_token));
	t->type = LABEL;
	t->instr = if_its_instr(str);
	t->next = NULL;
	t->label = a->current_label;
	t->args[0] = NULL;
	t->args[1] = NULL;
	t->args[2] = NULL;
	return (t);
}

void		label_to_tokens(t_asm *a, char *str)
{
	t_token		*tmp_token;

	tmp_token = a->tokens;
	if(a->tokens)
	{
		while (tmp_token->next)
			tmp_token = tmp_token->next;
		tmp_token->next = init_label(a, tmp_token->next, str);
		// tmp_token->next = (t_token *)malloc(sizeof(t_token));
		// tmp_token->next->type = LABEL;
		// tmp_token->next->instr = if_its_instr(str);
		// tmp_token->next->next = NULL;
		// tmp_token->next->label = a->current_label;
		// printf("[%s] TOKEN TYPE IS [LABEL] (%d), %d\n", str, tmp_token->next->type, tmp_token->next->instr);
	}
	else
	{
		a->tokens = init_label(a, a->tokens, str);
		// a->tokens = (t_token *)malloc(sizeof(t_token));
		// a->tokens->type = LABEL;
		// a->tokens->instr = if_its_instr(str);
		// a->tokens->next = NULL;
		// a->tokens->label = a->current_label;
		// printf("[%s] TOKEN TYPE IS [LABEL] (%d), %d\n", str, a->tokens->type, a->tokens->instr);
	}
	skip_label_if_instr_ferther(a, str);
	// printf("[[[[[%s]]]]]\n", str);
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
		free(str);
		str = NULL;
		a->current_line_number++;
	}
	if (str)
		free(str);
	return (1);
}


// void		put_labels_to_asm(char *filename)
// {
// 	int		fd;
// 	char	*str;
// 	char	*trimmed;

// 	fd = open(filename, O_RDONLY);
// 	while (get_next_line(fd, &str))
// 	{
// 		trimmed = ft_strtrim(str);
// 		if (trimmed[0] && !if_its_instr(trimmed) && if_l_chars(str))
// 			printf("THE LABEL IS [%s]\n", str); // TODO: Это ещё нужно вкинуть в список меток
// 		free(str);
// 		free(trimmed);
// 	}
// 	free(str);
// 	close(fd);
// }

int			main(int argc, char **argv)
{
	int		fd;
	char	str[BUFF_SIZE + 1];
	t_asm	a;
	t_token	*tmp;

	a.f = 0;
	a.tokens = NULL;
	a.current_label = NULL;
	if (argc < 2)
		ft_putendl("Usage: ./vm_champs/asm <sourcefile.s>");
	else
	{
		// put_labels_to_asm(argv[argc - 1], &a);
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
