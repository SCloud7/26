/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 02:06:03 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/28 02:29:51 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_node(t_commandlist *mini, char *env, t_lst *lst)
{
	t_lst	*newnode;
	t_lst	*last;

	(void)mini;
	if (!lst)
		return ;
	newnode = malloc(sizeof(t_lst));
	if (newnode == NULL)
		return ;
	newnode->next = NULL;
	newnode->line = ft_strdup(env);
	newnode->pre = NULL;
	if (lst == NULL)
		lst = newnode;
	else
	{
		last = find_last(lst);
		if (last)
		{
			last->next = newnode;
			newnode->pre = last;
		}
	}
}

int	is_at_end(char *content, int i)
{
	int	in_double_quote;
	int	in_single_quote;

	in_double_quote = 0;
	in_single_quote = 0;
	if (content[i] == '\0')
		return (1);
	while (content[i] && (in_single_quote || in_double_quote))
	{
		if (content[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (content[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		i++;
	}
	return (content[i] == '\0' || (content[i] == '"' && !in_single_quote)
		|| (content[i] == '\'' && !in_double_quote));
}

void	handle_exit_status(t_commandlist *mini, char *expanded, int *j)
{
	char	*exit_char;
	int		i;
	int		k;

	exit_char = NULL;
	i = mini->res;
	k = 0;
	exit_char = ft_itoa(i);
	if (!exit_char)
		return ;
	while (exit_char[k])
		expanded[(*j)++] = exit_char[k++];
	free(exit_char);
}

void	handle_quotes(char *expanded, char *content, t_expand *exp)
{
	if (content[exp->i] == '"' && !exp->in_single_quote)
	{
		exp->in_double_quote = !exp->in_double_quote;
		exp->i++;
	}
	else if (content[exp->i] == '\'' && !exp->in_double_quote)
	{
		exp->in_single_quote = !exp->in_single_quote;
		exp->i++;
	}
	else if ((content[exp->i] == '"' && exp->in_single_quote) || (content[exp->i] == '\'' && exp->in_double_quote))
		expanded[exp->j++] = content[exp->i++];
}

