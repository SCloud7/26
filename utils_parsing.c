/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:57:40 by fsingh            #+#    #+#             */
/*   Updated: 2025/02/04 19:13:46 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ispace(char c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\f' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int is_special(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int only_space(char *input)
{
	int i;

	i = 0;
	if (!input || input[0] == '\0')
	{
		//printf("\n");
		return (1);
	}
	while (input[i])
	{
		if (!ispace(input[i]))
			return (0);
		i++;
	}
	printf("\n");
	return (1);
}

char *ft_strndup(const char *str, size_t n)
{
	char *res;
	size_t i;

	i = 0;
	res = (char *)malloc(n + 1);
	if (res == NULL)
		return (NULL);
	while (i < n && str[i] != '\0')
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int	open_quote(char *input)
{
	int		i;
    int		in_single_quote;
    int		in_double_quote;

	in_double_quote = 0;
	in_single_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		i++;
	}
	if (in_single_quote || in_double_quote)
	{
		printf("Syntax error: unclosed quote\n");
		return (1);
	}
	return (0);
}
