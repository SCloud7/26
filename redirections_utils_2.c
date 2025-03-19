/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:09:10 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/19 16:24:47 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_cmd(char **tab, char **tabb)
{
	printf("%s: command not found\n", tabb[0]);
	free_tab(tab);
	free_tab(tabb);
	//ft_exit(mini, NULL);
}

void	clean_lst(t_lst *lst)
{
	t_lst	*cur;
	t_lst	*next;

	cur = lst;
	while (cur)
	{
		next = cur->next;
		free(cur->line);
		free(cur);
		cur = next;
	}
	free(lst);
}

int	ft_strnstri(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	a;

	i = 0;
	if (!*needle)
		return (-1);
	while (haystack[i] && i < len)
	{
		a = 0;
		while (haystack[i + a] && needle[a] && haystack[i + a]
			&& len > i + a && haystack[i + a] == needle[a])
			a++;
		if (!needle[a])
			return (1);
		i++;
	}
	return (-1);
}

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab && tab[i])
		free(tab[i++]);
	free(tab);
	tab = NULL;
}

void	her_doc_p2(t_arg *fil, char *line, t_lst **lst)
{
	while (fil)
	{
		while (1)
		{
			line = readline("hd:");
			if (!line)
			{
				printf("heredoc: unexpected EOF\n");
				break ;
			}
			if (strcmp(line, fil->content) == 0)
			{
				free(line);
				break ;
			}
			append_node(NULL, line, *lst);
			free(line);
		}
		fil = fil->next;
	}
	redirect_lines(*lst);
}
