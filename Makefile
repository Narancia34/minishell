# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/08 11:31:47 by mlabrirh          #+#    #+#              #
#    Updated: 2025/04/08 11:33:34 by mlabrirh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
NAME = minishell

SRCS = $(shell ls ./sources/lexer/*.c) \
	   $(shell ls ./sources/parser/*.c) \
	   $(shell ls ./sources/expander/*.c) \
	   $(shell ls ./exec/*.c) \
	   $(shell ls ./sources/main.c)
OFILES = $(SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OFILES)
	$(CC) $(OFILES) $(LIBFT) $(LDFLAGS) -o $(NAME) 
	rm -f $(OFILES)
	make -C $(LIBFT_DIR) clean

clean:
	rm -f $(OFILES)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
