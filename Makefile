# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/07 13:11:47 by kri-              #+#    #+#              #
#    Updated: 2026/09/07 13:12:23 by kri-             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion.a
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I .

SRC = main.c

OBJS = ${SRC:.c=.o}

$(NAME): ${OBJS}
		ar rcs ${NAME} ${OBJS}

all:	${NAME}

#bonus: all

clean:
		${RM} ${OBJS}

fclean: clean
		${RM} ${NAME}

re: fclean all