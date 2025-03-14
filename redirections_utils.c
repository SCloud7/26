/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:06:14 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/13 16:20:02 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_len(t_command *cmd)
{
	int			i;
	t_command	*cur;

	cur = cmd;
	i = 0;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

int	list_len(t_lst **lst)
{
	int		i;
	t_lst	*cur;

	i = 0;
	cur = *lst;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

char	**lst_to_tab(t_lst **lst)
{
	int		i;
	int		len;
	char	**tab;
	t_lst	*cur;

	if (!lst || !*lst)
		return (NULL);
	len = list_len(lst);
	cur = *lst;
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (cur)
	{
		tab[i] = ft_strdup(cur->line);
		cur = cur->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

char	**arg_to_tab(t_arg *lst)
{
	int		i;
	char	**tab;
	t_arg	*cur;

	i = arg_len(&lst);
	tab = malloc(sizeof(char *) * (i + 1));
	i = 0;
	cur = lst;
	if (!tab)
		return (NULL);
	while (cur)
	{
		tab[i] = ft_strdup(cur->content);
		cur = cur->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

int	arg_len(t_arg **ar)
{
	int		i;
	t_arg	*cur;

	i = 0;
	cur = *ar;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}
