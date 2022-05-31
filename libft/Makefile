# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alcristi <alcristi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/09 23:05:05 by alcristi          #+#    #+#              #
#    Updated: 2021/12/13 22:28:07 by alcristi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


FLAGS		= -Wall -Wextra -Werror
COMP		= gcc
NAME		= libft.a
INCLUDE		= libft.h
FLAGLIB1	= ar -rc
FLAGLIB2	= ranlib
RM			= /bin/rm -f

FUNC		= ft_atoi.c ft_calloc.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c ft_toupper.c ft_tolower.c ft_striteri.c\
			ft_strlen.c ft_strlcpy.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strdup.c ft_strncmp.c ft_strlcpy.c ft_strlcat.c ft_strnstr.c ft_substr.c ft_strtrim.c ft_strmapi.c ft_strjoin.c\
			ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_itoa.c ft_split.c\
			get_next_line.c



FUNC_B		= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c\
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c\
			ft_lstmap.c



OBJ			= $(FUNC:.c=.o)

OBJ_B		= $(FUNC_B:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ) $(INCLUDE)
			$(FLAGLIB1) $(NAME) $(OBJ)
			$(FLAGLIB2) $(NAME)

bonus:		$(OBJ_B)
			$(FLAGLIB1) $(NAME) $(OBJ_B)
			$(FLAGLIB2) $(NAME)


%.o: %.c
			$(COMP) -I. -o $@ -c $? $(FLAGS)

clean:
			$(RM) $(OBJ) $(OBJ_B)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

rebonus:	fclean bonus

.PHONY:		all re bonus clean fclean rebonus
