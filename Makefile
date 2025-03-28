# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/02 16:48:52 by ssoukoun          #+#    #+#              #
#    Updated: 2025/02/04 16:23:10 by fsingh           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
HEAD = .
CFLAGS = -g3 -Wall -Werror -Wextra
LDFLAGS = -lreadline  # Ajout du linking readline
SOURCES = main.c parsing.c free_shell.c utils_parsing.c utils_parsing_2.c utils_parsing_3.c env_init.c expand_length.c parsing_2.c env_management.c builds_in.c exe.c \
		redirections_utils_3.c redirections_utils_2.c redirections_utils.c redirections.c utils.c expand_utils.c
OBJECTS = $(SOURCES:.c=.o)  # Génération des fichiers .o automatiquement


MAKE_LIBFT        =    make -s -C ./libft
LIBFT_A			=		./libft/libft.a

$(LIBFT_A):
	$(MAKE_LIBFT)

all: $(NAME) $(LIBFT_A)

$(NAME): $(OBJECTS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJECTS) ${LIBFT_A} -g -o $(NAME) $(LDFLAGS)  # Link avec -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HEAD)  # Compilation des .c en .o

clean:
	rm -rf $(OBJECTS)
	$(MAKE_LIBFT) clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE_LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re 
