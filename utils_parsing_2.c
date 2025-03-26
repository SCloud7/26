/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:51:41 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/26 20:18:09 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirection_syntax(char *input, int *i)
{
	char	temp;

	temp = input[*i];
	(*i)++;
	if (input[*i] == temp)
		(*i)++;
	while (isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\0' || input[*i] == '>'
		|| (input[*i] == '<' && input[*i + 1] != '<') || input[*i] == '|')
	{
		printf("Syntax error near unexpected token \n");
		return (2);
	}
	return (0);
}

int	check_pipe_syntax(char *input, int *i)
{
	(*i)++;
	while (isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\0' || input[*i] == '>'
		|| input[*i] == '<' || input[*i] == '|')
	{
		printf("Syntax error near unexpected token `|'\n");
		return (2);
	}
	return (0);
}

int	checking_syntax(char *input)
{
	int	i;
	int	quote;

	i = 0;
	if (input[i] == '|' || (input[i] == '<' && input[i + 1] != '<')
		|| input[i] == '>')
	{
		printf("Syntax error near unexpected token `%c`\n", input[i]);
		return (2);
	}
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			i++;
		}
		else if (input[i] == '>' || input[i] == '<')
		{
			if (check_redirection_syntax(input, &i))
				return (2);
		}
		else if (input[i] == '|')
		{
			if (check_pipe_syntax(input, &i))
				return (2);
		}
		else
			i++;
	}
	return (0);
}

int	checking_error_before(char *input)
{
	return (only_space(input) || open_quote(input) || checking_syntax(input));
}
