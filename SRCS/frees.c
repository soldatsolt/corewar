#include "corewar.h"

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
	if (er_flag == 1) // er_flag = 1 если ошибка и он выходит
		ft_printf("LEX ERROR ON LINE [%d]\n", a->current_line_number);
	if (er_flag == 2) // er_flag = 1 если ошибка и он выходит
		ft_printf("NO CODE IN FILE\n", a->current_line_number);
	if (to_free)
		free(*to_free);
	exit(1);
}
