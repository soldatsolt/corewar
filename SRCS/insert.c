#include "corewar.h"

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
	insert_all_between_quotes_to_str(fd, str, COMMENT_LENGTH, a);
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
	insert_all_between_quotes_to_str(fd, str, PROG_NAME_LENGTH, a);
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
	}
	else
	{
		a->tokens = init_instruction(a->tokens, str);
	}
	a->exec_code_size++;
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
	}
	else
	{
		a->tokens = init_label(a, a->tokens, str);
	}
	skip_label_if_instr_ferther(a, str);
}
