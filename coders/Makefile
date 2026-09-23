# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/07 13:11:47 by kri-              #+#    #+#              #
#    Updated: 2026/09/18 15:49:02 by kri-             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDE = -I .

SRC = main.c

OBJS_DIR = obj
OBJS = $(SRC:%.c=$(OBJS_DIR)/%.o)

all:	${NAME}

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
		${RM} -r ${OBJS_DIR}

fclean: clean 
		${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re