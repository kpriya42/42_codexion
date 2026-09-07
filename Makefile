# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/07 13:11:47 by kri-              #+#    #+#              #
#    Updated: 2026/09/07 17:07:26 by kri-             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDE = -I .

SRC = main.c

OBJS = ${SRC:.c=.o}

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

all:	${NAME}

clean:
		${RM} ${OBJS}

fclean: clean 
		${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re