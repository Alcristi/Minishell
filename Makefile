FLAGS		= -Wall -Wextra -Werror
FLAGSMINI	= -lreadline ./libft/libft.a
COMP		= gcc
NAME		= minishell
RM			= /bin/rm -f

FUNC		=  	src/aux.c src/aux_temp.c src/build_cmd_aux.c src/build_cmd.c src/dir.c src/error_validator.c src/execute_aux.c \
				src/execute_builtin_aux.c src/execute_builtin.c src/execute.c src/execute_pipe_aux.c src/execute_pipe.c src/here_doc.c \
				src/linked_list_method.c src/main.c src/parse.c src/prompt_aux.c src/prompt.c src/redirect.c src/resolve_dollar_aux.c \
				src/resolve_dollar.c src/resolve_d_quotes.c src/resolve_s_quotes.c src/resolve_string_aux.c src/resolve_string.c src/signals.c \
				src/stackTokens.c src/token.c src/tokenization_aux.c src/tokenization.c src/validation_parse.c src/validator_aux.c \
				builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c builtins/export.c builtins/pwd.c builtins/unset.c
OBJ			= $(FUNC:.c=.o)

OBJ_B		= $(FUNC_B:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ) $(INCLUDE)
			make -C libft
			$(COMP) $(FLAGS)  $(FUNC) $(FLAGSMINI) -o $(NAME)

%.o: %.c
			$(COMP) -o $@ -c $? $(FLAGS)

clean:
			$(RM) $(OBJ) $(OBJ_B)
			make -C libft clean

fclean:		clean
			$(RM) $(NAME)
			make -C libft fclean

re:			fclean all

rebonus:	fclean bonus

.PHONY:		all re bonus clean fclean rebonus
