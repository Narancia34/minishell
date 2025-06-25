# Colors
GREEN  := \033[0;32m
RED    := \033[0;31m
YELLOW := \033[1;33m
BLUE   := \033[1;34m
RESET  := \033[0m

CC = cc
FLAGS = -Wall -Werror -Wextra
NAME = libft.a

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
	   ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c \
	   ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
	   ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c \
	   ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
	   ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c \
	   ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
	   ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
	   ft_putendl_fd.c ft_putnbr_fd.c ft_whitespace.c ft_space.c ft_is_operator.c ft_strcmp.c ft_strndup.c

BSRCS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

OFILES = $(SRCS:.c=.o)
BOFILES = $(BSRCS:.c=.o)

.SILENT:

all: $(NAME)

$(NAME): $(OFILES)
	@ar rcs $(NAME) $(OFILES)
	@echo "$(GREEN)Compiled successfully!$(RESET)"

%.o: %.c
	@printf "$(YELLOW)Compiling $< ...$(RESET) "
	@if $(CC) -c $(FLAGS) $< -o $@ 2> $@.err; then \
		printf "$(GREEN)[OK]$(RESET)\n"; \
		rm -f $@.err; \
	else \
		printf "$(RED)[KO]$(RESET)\n"; \
		cat $@.err; \
		rm -f $@.err; \
		exit 1; \
	fi

bonus: $(BOFILES)
	@ar rcs $(NAME) $(BOFILES)
	@echo "$(GREEN)Bonus compiled successfully!$(RESET)"

clean:
	@rm -rf $(OFILES) $(BOFILES)
	@echo "$(BLUE)Objects cleaned.$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(BLUE)All cleaned.$(RESET)"

re: fclean all

.PHONY: all bonus clean fclean re
