/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmills <kmills@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/19 19:20:50 by kmills            #+#    #+#             */
/*   Updated: 2019/09/19 19:20:50 by kmills           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H
# include "../libft/libft.h"
# include <sys/stat.h>
# include <sys/xattr.h>
# include "op.h"
# include <time.h>
# include <dirent.h>
# include <sys/types.h>
# include <pwd.h>
# include <uuid/uuid.h>
# include <grp.h>
# include <uuid/uuid.h>
# define F_NAME 0x1
# define F_COMMENT 0x2
# define CNTL "Champion name too long (Max length 2048)"
# define CCTL "Champion comment too long (Max length 2048)"

typedef enum
{
						LABEL,
						INSTRUCTION,
						REGISTER,
						DIRECT,
						INDIRECT,
						SEPARATOR,
						DIRECT_LABEL,
						INDIRECT_LABEL,
						NEXT_LINE,
						ERR
}						t_type;

typedef struct          s_op
{
	char				*name;
	int					n_args;
	int					args[3];
	int					op_num;
	int					cycle;
	char				*descr;
	int					code_arg_type;
	int					hz2;
}						t_op;

typedef struct          s_token
{
	t_type				type;
	int					instr;
	char				*label;
	char				*args[3];
	struct s_token		*next;
}						t_token;

typedef struct          s_asm
{
	char				name[PROG_NAME_LENGTH + 1];
	char				comment[COMMENT_LENGTH + 1];
	int					f; // - Флаги. K примеру, было ли имя объявлено ранее или нет и тд.
	int					current_line_number;
	char				*current_label;
	int					current_instruction;
	int					exec_code_size;
	t_token				*tokens;
}						t_asm;

t_token		*init_instruction(t_token *t, char *str);
t_token		*init_label(t_asm *a, t_token *t, char *str);
t_asm		init_asm(void);
int			if_its_instr(char *str);
void		u_lose(int need_free, char *to_free, char *note, t_asm *a);
void		free_token_content(t_token *t);
void		free_all_tokens(t_token *t);
void		free_parse_exit(t_asm *a, int er_flag, char **to_free);
void		parse_args_instr(t_asm *a, char *str, char **to_free);
void		skip_label_if_instr_ferther(t_asm *a, char *str);
void		skip_whitespaces(char	**str);
void		instr_to_tokens(t_asm *a, char *str);
void		insert_all_between_quotes_to_str(int fd, char *str, int limit, t_asm *a);
void		insert_comment_to_asm(int fd, char *str, t_asm *a);
void		insert_name_to_asm(int fd, char *str, t_asm *a);
void		instr_to_tokens(t_asm *a, char *str);
void		label_to_tokens(t_asm *a, char *str);
int			first_non_space_char(char *str);
char		*last_instr_name(t_asm *a);
t_token		*last_instr(t_asm *a);
int			first_space_char(char *str);
char		*make_str_withnocomment(char *str);


#endif
