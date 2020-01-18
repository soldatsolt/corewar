#include "corewar.h"

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
