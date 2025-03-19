/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:20:05 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/19 18:01:08 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2, int j)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && i < j)
		i++;
	if (s1[i] != s2[i])
	{
		return (-1);
	}
	return (0);
}

void	redirect_lines(t_lst *lst)
{
	int		pipes[2];
	t_lst	*act;

	act = lst;
	if (pipe(pipes) < 0)
		return ;
	while (act)
	{
		write(pipes[1], act->line, ft_strlen(act->line));
		write(pipes[1], "\n", 1);
		act = act->next;
		printf("line = %s\n", act->line);
	}
	dup2(pipes[0], STDIN_FILENO);
	close(pipes[0]);
	close(pipes[1]);
	clean_lst(lst);
}
