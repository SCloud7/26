/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:04:36 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/24 15:15:21 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	red(t_command *cur, t_commandlist *mini)
{
	verif_pipe(cur);
	her_doc(cur, mini);
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

void	her_doc(t_command *cur, t_commandlist *mini)
{
	t_arg	*fil;

	fil = cur->heredoc;
	if (!fil)
		return ;
	if (fil->content == NULL || fil->content[0] == '\0')
		ft_exit(mini, NULL);
	her_doc_p2(fil);
}
