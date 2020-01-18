#include "corewar.h"

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

t_asm		init_asm(void)
{
	t_asm	a;

	a.f = 0;
	a.tokens = 0;
	a.current_instruction = 0;
	a.current_label = NULL;
	return (a);
}
