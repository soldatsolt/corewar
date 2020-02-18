#include "corewar.h"

void		skip_label_if_instr_ferther(t_asm *a, char *str)
{
	void	*to_free;

	to_free = (void *)str;
	while (ft_strchr(LABEL_CHARS, *str))
		str++;
	str++;
	while (*str == '\t' || *str == ' ')
		str++;
	if (*str && if_its_instr(str))
	{
		a->current_instruction = if_its_instr(str);
		instr_to_tokens(a, str);
		parse_args_instr(a, str, (char **)&to_free);
	}
	else if (*str)
	{
		free_parse_exit(a, 1, (char **)&to_free);
	}
}

void		skip_whitespaces(char	**str)
{
	while(**str == ' ' || **str == '\t')
		(*str)++;
}
