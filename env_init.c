/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:31 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/16 21:31:30 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_numlen(int nbr)
{
	int	i;

	i = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		i++;
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		i++;
		nbr /= 10;
	}
	return (i);
}

void	set_env(t_commandlist *mini, char **env)
{
	int		i;
	t_lst	*newnode;

	i = 0;
	if (!env || !*env)
		exit(0);
	mini->res = 2;
	while (env[i])
	{
		newnode = malloc(sizeof(t_lst));
		if (!newnode)
			return ;
		newnode->line = ft_strdup(env[i]);
		newnode->next = NULL;
		newnode->pre = NULL;
		if (!mini->env)
			mini->env = newnode;
		else
		{
			t_lst *last = find_last(mini->env);
			last->next = newnode;
			newnode->pre = last;
		}
		i++;
	}
}

t_lst	*find_last(t_lst *stack)
{
	if (!stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}

