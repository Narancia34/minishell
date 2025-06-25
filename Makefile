# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Narancia34 <narancia34@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/25 13:33:00 by Narancia34        #+#    #+#              #
#    Updated: 2025/06/25 13:33:00 by Narancia34       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
GREEN  := \033[0;32m
RED    := \033[0;31m
YELLOW := \033[1;33m
BLUE   := \033[1;34m
RESET  := \033[0m

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

# Hide command output
.SILENT:

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

%.o: %.c
	@printf "$(YELLOW)Compiling $< ...$(RESET) "
	@if $(CC) $(CFLAGS) -c $< -o $@ 2> $@.err; then \
		printf "$(GREEN)[OK]$(RESET)\n"; \
		rm -f $@.err; \
	else \
		printf "$(RED)[KO]$(RESET)\n"; \
		cat $@.err; \
		rm -f $@.err; \
		exit 1; \
	fi

$(NAME): $(OFILES)
	@$(CC) $(OFILES) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo ""
	@echo "$(GREEN)Compiled successfully!$(RESET)"

clean:
	@rm -f $(OFILES)
	@make -C $(LIBFT_DIR) clean
	@echo "$(BLUE)Objects cleaned.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(BLUE)All cleaned.$(RESET)"

re: fclean all

.PHONY: all clean fclean re

# CC = cc
# CFLAGS = -Wall -Wextra -Werror
# LDFLAGS = -lreadline
# NAME = minishell
#
# SRCS = ./sources/lexer/build_commands.c  ./sources/lexer/build_utils.c  ./sources/lexer/tokens.c  ./sources/lexer/utils_tokens.c \
#        ./sources/expander/expander.c  ./sources/expander/expander_utils1.c  ./sources/expander/expander_utils2.c \
#        ./sources/parser/valid_syntax.c \
#        ./exec/cd.c ./exec/cd_utils.c ./exec/check_input.c ./exec/check_input_utils.c ./exec/echo.c ./exec/env.c ./exec/execute_cmd.c \
#        ./exec/execute_cmd_utils.c ./exec/exit.c ./exec/export.c ./exec/export_utils.c ./exec/free_lists.c ./exec/hande_paths.c \
#        ./exec/handle_builtins.c ./exec/handle_paths_utils.c ./exec/handle_pipes.c ./exec/handle_pipes_utils.c ./exec/handle_shlvl.c \
#        ./exec/handle_var.c \
#        ./exec/here_doc.c ./exec/here_doc_utils.c ./exec/make_env.c ./exec/pwd.c ./exec/redirection_utils.c ./exec/redirections.c ./exec/signals.c \
#        ./exec/store_pid.c ./exec/unset.c ./exec/upd_env.c \
#        ./sources/main.c ./sources/utils.c
#
#
# OFILES = $(SRCS:.c=.o)
#
# LIBFT_DIR = ./libft
# LIBFT = $(LIBFT_DIR)/libft.a
#
# all: $(LIBFT) $(NAME)
#
# $(LIBFT):
# 	make -C $(LIBFT_DIR)
#
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@
#
# $(NAME): $(OFILES)
# 	$(CC) $(OFILES) $(LIBFT) $(LDFLAGS) -o $(NAME) 
#
# clean:
# 	rm -f $(OFILES)
# 	make -C $(LIBFT_DIR) clean
#
# fclean: clean
# 	rm -f $(NAME)
# 	make -C $(LIBFT_DIR) fclean
#
# re: fclean all
#
# .PHONY: all clean fclean re
#
