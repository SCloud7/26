/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:51:41 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/28 02:24:45 by fsingh           ###   ########.fr       */
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
		write(2, "Syntax error near unexpected token \n", 37);
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
		write(2, "Syntax error near unexpected token `|'\n", 40);
		return (2);
	}
	return (0);
}

int	check_start_syntax(char *input)
{
	if (input[0] == '|' || (input[0] == '<' && input[1] != '<')
		|| input[0] == '>')
	{
		write(2, "Syntax error near unexpected token\n", 36);
		return (2);
	}
	return (0);
}

int	checking_syntax(char *input)
{
	int	i;

	if (check_start_syntax(input))
		return (2);
	i = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			skip_quotes_parse(input, &i);
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

int	checking_error_before(t_commandlist *mini, char *input)
{
	if (only_space(input))
		return (1);
	if (open_quote(input))
		return (1);
	if (checking_syntax(input))
	{
		mini->res = 2;
		return (1);
	}
	return (0);
}
