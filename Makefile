# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/08 11:31:47 by mlabrirh          #+#    #+#              #
#    Updated: 2025/06/24 14:25:27 by mgamraou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
NAME = minishell

SRCS = ./sources/lexer/build_commands.c  ./sources/lexer/build_utils.c  ./sources/lexer/tokens.c  ./sources/lexer/utils_tokens.c \
       ./sources/expander/expander.c  ./sources/expander/expander_utils1.c  ./sources/expander/expander_utils2.c \
       ./sources/parser/valid_syntax.c \
       ./exec/cd.c ./exec/cd_utils.c ./exec/check_input.c ./exec/check_input_utils.c ./exec/echo.c ./exec/env.c ./exec/execute_cmd.c \
       ./exec/execute_cmd_utils.c ./exec/exit.c ./exec/export.c ./exec/export_utils.c ./exec/free_lists.c ./exec/hande_paths.c \
       ./exec/handle_builtins.c ./exec/handle_paths_utils.c ./exec/handle_pipes.c ./exec/handle_pipes_utils.c ./exec/handle_shlvl.c \
       ./exec/handle_var.c \
       ./exec/here_doc.c ./exec/here_doc_utils.c ./exec/make_env.c ./exec/pwd.c ./exec/redirection_utils.c ./exec/redirections.c ./exec/signals.c \
       ./exec/store_pid.c ./exec/unset.c ./exec/upd_env.c \
       ./sources/main.c ./sources/utils.c


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

clean:
	rm -f $(OFILES)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

