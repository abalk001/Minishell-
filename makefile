NAME		=	minishell

MAIN		=	main.c main_tools.c

HEREDOC 	=	heredoc/heredoc.c heredoc/heredoc_tools_2.c \
				heredoc/heredoc_tools.c

BUILT_IN	=	built_in/cd_local.c built_in/echo_local.c \
				built_in/env.c built_in/export.c \
				built_in/unset_local.c built_in/pwd_local.c \
				built_in/exit_local.c built_in/transform.c

EXECUTION	=	execution/execution.c execution/tools_1.c \
				execution/tools_2.c execution/exec_2.c \
				execution/tools_3.c execution/tools_4.c

PARSING		=	parsing/argument.c parsing/builder.c \
				parsing/parsing.c parsing/redirection.c \
				parsing/tokenization.c parsing/tools.c \
				parsing/parsing_memory.c parsing/file.c \
				parsing/tok_tools.c

UTILS		=	utils/clone_env.c utils/expand.c \
				utils/free.c utils/signals.c \
				utils/tools.c utils/tools_1.c \
				utils/tools_string.c utils/print.c \
				utils/export_utils.c

SRCS		=	${BUILT_IN} \
				${EXECUTION} \
				${PARSING} \
				${UTILS} \
				${MAIN} \
				${HEREDOC}

OBJS 		=	${SRCS:.c=.o}

HEADER		=	-I.

CC 			=	cc
CFLAGS 		=	-Wall -Wextra -Werror

all:			${NAME}

${NAME}:		${OBJS}
	@make re -C ./libft
	@$(CC) ${OBJS} -Llibft -lft -lreadline -o ${NAME}

%.o:			%.c $(HEADER)
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@make clean -C ./libft
	@rm -f ${OBJS}

fclean:			clean
	@make fclean -C ./libft
	@rm -f $(NAME)
	@rm -f ${NAME}

re:				fclean all clean

.PHONY: 		all clean fclean re ${NAME}