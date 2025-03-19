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

NAME = mini
CC = cc
HEAD = .
CFLAGS = -g3 -Wall -Werror -Wextra
LDFLAGS = -lreadline  # Ajout du linking readline
SOURCES = main.c parsing.c free_shell.c utils_parsing.c utils_parsing_2.c utils_parsing_3.c env_init.c expand_length.c parsing_2.c env_management.c builds_in.c exe.c \
		redirections_utils_3.c redirections_utils_2.c redirections_utils.c redirections.c
OBJECTS = $(SOURCES:.c=.o)  # Génération des fichiers .o automatiquement

LIBFT_A = ./libft/libft.a

all: $(LIBFT_A) $(NAME)

$(LIBFT_A):
	$(MAKE) -s -C ./libft

$(NAME): $(OBJECTS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJECTS) ${LIBFT_A} -g -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HEAD)

clean:
	rm -rf $(OBJECTS)
	$(MAKE) -C ./libft clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re 
