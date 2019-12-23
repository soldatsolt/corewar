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

typedef enum			e_type
{
						LABEL,
						INSTRUCTION,
						REGISTER,
						SSEPARATOR,
						DIRECT_LABEL,
						SEPARATOR,
						DIRECT
}						t_type;

typedef struct          s_asm
{
	char				name[PROG_NAME_LENGTH + 1];
	char				comment[COMMENT_LENGTH + 1];
	int					f; // - Флаги. K примеру, было ли имя объявлено ранее или нет и тд.
	int					current_line_number;
}						t_asm;

#endif
