/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:56:48 by fsingh            #+#    #+#             */
/*   Updated: 2025/02/04 19:03:10 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		if (input[*i] == '\0')
			return (1);
	}
	if (input[*i] == '\0')
		return (1);
	(*i)++;
	return (0);
}

int	handle_arguments(char *input, int *i, t_commandlist *mini)
{
	int		start;
	char	*current_token;

	current_token = NULL;
	start = *i;
	while (input[*i] && !is_special(input[*i]) && !isspace(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (skip_quotes(input, i))
				return (1);
		}
		else
			(*i)++;
		if (input[*i] == '\0')
			break ;
	}
	current_token = ft_strndup(&input[start], *i - start);
	if (!current_token)
		return (1);
	add_token(mini, TOKEN_ARG, current_token);
	free(current_token);
	while (input[*i] && isspace(input[*i]))
		(*i)++;
	return (0);
}

void	handle_special_token(char *input, int *i, t_commandlist *mini)
{
	if (input[*i] == '|')
		handle_pipe(input, i, mini, TOKEN_PIPE);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		handle_double_redirect(input, i, mini, TOKEN_APPEND);
	else if (input[*i] == '<' && input[*i + 1] == '<')
		handle_double_redirect(input, i, mini, TOKEN_HEREDOC);
	else if (input[*i] == '>')
		handle_single_redirect(input, i, mini, TOKEN_OUT);
	else if (input[*i] == '<')
		handle_single_redirect(input, i, mini, TOKEN_IN);
}

int	lexing(char *input, t_commandlist *mini)
{
	int		i;

	mini->tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (is_special(input[i]))
			handle_special_token(input, &i, mini);
		else if (handle_arguments(input, &i, mini))
			return (1);
		while (isspace(input[i]))
			i++;
	}
	return (0);
}

int	parsing(char *input, t_commandlist *mini)
{
	if (checking_error_before(mini, input))
		return (1);
	if (lexing(input, mini) == 1)
		return (1);
	if (parse_token(mini) == 1)
		return (1);
	expand_variables(mini);
	build_in(mini, input);
	return (0);
}
