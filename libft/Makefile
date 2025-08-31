CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
NAME	=	libft.a
SRC		=	ft_isalnum.c ft_isalpha.c \
			ft_isdigit.c ft_memcpy.c \
			ft_putchar_fd.c ft_putstr_fd.c \
			ft_split.c ft_strchr.c \
			ft_strdup.c ft_strjoin.c \
			ft_strlcpy.c ft_strlen.c \
			ft_strncmp.c ft_strrchr.c \
			ft_substr.c ft_strlcat.c \
			ft_itoa.c
OBJ		=	$(SRC:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
	ar rc $(NAME) $(OBJ)

%.o:		%.c 
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "all object files are deleted"

fclean:		clean
	@rm -f $(NAME)

re:			fclean all

.PHONY: 	all clean fclean re ${NAME}
