/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:11:14 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/24 14:55:29 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	waiting_room(int *id, int j)
{
	int	i;
	int	status;
	int	last_exit_status;

	last_exit_status = 0;
	i = 0;
	status = 0;
	while (i < j)
	{
		waitpid(id[i], &status, 0);
		if (WIFEXITED(status))
		{
			last_exit_status = WEXITSTATUS(status);
			if (last_exit_status != 0)
				break;
		}
		i++;
	}
	return (last_exit_status);
}

int	cr_fork(void)
{
	int	i;

	i = fork();
	if (i < 0)
	{
		perror("fork");
	}
	return (i);
}

void	close_pip(t_command *cur)
{
	if (cur->prev)
		close(cur->prev->pipe[0]);
	if (cur->next)
		close(cur->pipe[1]);
}

void	cr_pip(t_command *cur)
{
	if (cur->next)
	{
		if (pipe(cur->pipe) == -1)
		{
			perror("pipe");
			return ;
		}
	}
}

int	lefork(void)
{
	int	i;

	i = fork();
	if (i < 0)
		return (perror("fork1"), -1);
	return (i);
}
