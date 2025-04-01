/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:31 by fsingh            #+#    #+#             */
/*   Updated: 2025/04/01 04:39:10 by fsingh           ###   ########.fr       */
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
	t_lst	*last;

	i = 0;
	if (!env || !*env)
		exit(0);
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
			last = find_last(mini->env);
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

void	expand_exit_lenght(t_commandlist *mini, t_expand_lenght *exp_l)
{
	exp_l->new_len += ft_numlen(mini->res);
	exp_l->i++;
}

void	toggle_quote(char c, int *in_double_quote, int *in_single_quote)
{
	if (c == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
	else if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
}
