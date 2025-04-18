/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:54:35 by fsingh            #+#    #+#             */
/*   Updated: 2025/04/01 04:36:18 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_single_redirect(char *input, int *i, t_commandlist *mini,
			t_token_type token_type)
{
	int		start;
	char	*file;

	(*i) += 1;
	skip_whitespace(input, i);
	start = *i;
	while (input[*i] && !is_special(input[*i]) && !isspace(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (skip_quotes(input, i))
				return ;
		}
		else
			(*i)++;
		if (input[*i] == '\0')
			break ;
	}
	if (start == *i)
		return ;
	file = ft_strndup(&input[start], *i - start);
	add_token(mini, token_type, file);
	free (file);
	skip_whitespace(input, i);
}

void	handle_pipe(char *input, int *i, t_commandlist *mini,
			t_token_type token_type)
{
	add_token(mini, token_type, "|");
	(*i)++;
	while (input[*i] && isspace(input[*i]))
		(*i)++;
}

int	add_token(t_commandlist *mini, t_token_type type, char *token_value)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (1);
	new_token->type = type;
	new_token->value = ft_strdup(token_value);
	if (!new_token->value)
	{
		free (new_token);
		return (1);
	}
	new_token->next = NULL;
	if (!mini->tokens)
		mini->tokens = new_token;
	else
	{
		temp = mini->tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
	return (0);
}

void	handle_double_redirect(char *input, int *i, t_commandlist *mini,
			t_token_type token_type)
{
	int		start;
	char	*file;

	(*i) += 2;
	skip_whitespace(input, i);
	start = *i;
	while (input[*i] && !is_special(input[*i]) && !isspace(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (skip_quotes(input, i))
				return ;
		}
		else
			(*i)++;
		if (input[*i] == '\0')
			break ;
	}
	file = ft_strndup(&input[start], *i - start);
	add_token(mini, token_type, file);
	free (file);
	skip_whitespace(input, i);
}

void	skip_quotes_parse(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i])
		(*i)++;
}
