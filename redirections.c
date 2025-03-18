/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:04:36 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/19 00:28:44 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	red(t_command *cur)
{
	/*t_command	*prev;
	t_command	*next;

	prev = cur->prev;
	next = cur->next;*/
	her_doc(cur);
	in_file(cur);
	out_file(cur);
	add_file(cur);
}

void	in_file(t_command *cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur->infile;
	while (fil)
	{
		if (access(fil->content, F_OK) == -1)
		{
			printf("zsh: no such file or directory: %s \n", fil->content);
			return ;
		}
		if (access(fil->content, R_OK) == -1)
		{
			printf("zsh: permission denied: %s \n", fil->content);
			return ;
		}
		fd = open(fil->content, O_RDONLY, 0644);
		if (fd < 0)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDIN_FILENO);
		(close(fd), fil = fil->next);
	}
}

void	out_file(t_command *cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur->outfile;
	fd = -1;
	while (fil)
	{
		fd = open(fil->content, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (fd < 0 || access(fil->content, R_OK | W_OK) == -1)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fil = fil->next;
	}
}

void	add_file(t_command *cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur->append;
	fd = -1;
	while (fil)
	{
		fd = open(fil->content, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd < 0 || access(fil->content, R_OK | W_OK) == -1)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fil = fil->next;
	}
}

void	her_doc(t_command *cur)
{
	t_arg	*fil;
	char	*line;
	t_lst	*lst;

	lst = malloc(sizeof(t_lst *));
	if (!lst)
		return ;
	lst = NULL;
	fil = cur->heredoc;
	while (fil)
	{
		while (1)
		{
			line = readline("hd:");
			if (strcmp(line, fil->content) == 0)
			{
				free(line);
				break ;
			}
			append_node(NULL, line, lst);
			free(line);
		}
		redirect_lines(lst);
		fil = fil->next;
	}
}
