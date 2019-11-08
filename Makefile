MAKE=make
NAME=corewar
SRCS=SRCS/main.c
OBJS=$(SRCS:SRCS%.c=OBJS%.o)
HEADER=INCLUDES/corewar.h
INCLUDES=INCLUDES
LIBFT=libft/libft.a
FLAGS = -Wall -Wextra -Werror
LIBFTCFILES = libft/*.c
A_FILES=libft/libft.a libft/libftprintf.a
NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32;01m
ERROR_COLOR=\x1b[31;01m
WARN_COLOR=\x1b[33;01m
COMPILING_STRING=$(OK_COLOR)[KMILLS.FILLER COMPILED]$(NO_COLOR)
REMOVE_ALL_STRING=$(ERROR_COLOR)[KMILLS.FILLER REMOVED]$(NO_COLOR)
REMOVE_O_STRING=$(WARN_COLOR)[ALL .O REMOVED]$(NO_COLOR)
ECHO=echo
override G +=

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES) $(LIBFT)
	@gcc $(A_FILES) -I$(INCLUDES) $(OBJS) -o $(NAME)
	@$(ECHO) "$(COMPILING_STRING)"

$(LIBFT): $(LIBFTCILES)
	@$(MAKE) -C libft

OBJS/%.o: SRCS/%.c $(HEADER)
	@gcc -c $(FLAGS) $< -I$(INCLUDES) -I$(LIBFT) -o $@

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C libft clean
	@$(ECHO) "$(REMOVE_O_STRING)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean
	@$(ECHO) "$(REMOVE_ALL_STRING)"

git: clean
	git add .
	git commit -am "$(G)"
	git push

re: fclean all
