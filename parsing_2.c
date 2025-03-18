/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:52:32 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/19 00:43:14 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *init_command(void)
{
	t_command *new_command;
	
	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->args = NULL;
	new_command->infile = NULL;
	new_command->outfile = NULL;
	new_command->append = NULL;
	new_command->heredoc = NULL;
	new_command->next = NULL;
	return (new_command);
}

void	add_arg(t_arg **arg_list, char *str)
{
	t_arg *temp;
	t_arg *new_arg;
	
	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return;
	new_arg->content = strdup(str);
	if (!new_arg->content)
	{
		free(new_arg);
		return;
	}
	new_arg->next = NULL;
	if (!*arg_list)
		*arg_list = new_arg;
	else
	{
		temp = *arg_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_arg;
	}
}

void	add_token_cmd(t_command *current, t_token_type token_type, char *str)
{
	if (token_type == TOKEN_ARG)
		add_arg(&current->args, str);
	else if (token_type == TOKEN_IN)
		add_arg(&current->infile, str);
	else if (token_type == TOKEN_OUT)
		add_arg(&current->outfile, str);
	else if (token_type == TOKEN_APPEND)
		add_arg(&current->append, str);
	else if (token_type == TOKEN_HEREDOC)
		add_arg(&current->heredoc, str);
}

void	add_command_to_list(t_commandlist *mini, t_command *current)
{
	t_command *temp;
	
	if (!mini->cmd)
		mini->cmd = current;
	else
	{
		temp = mini->cmd;
		while (temp->next)
			temp = temp->next;
		temp->next = current;
		current->prev = temp;
	}
}

int	parse_token(t_commandlist *mini)
{
	t_token		*current_token;
	t_command	*current;

	current_token = mini->tokens;
	current = NULL;
	while (current_token)
	{
		if (!current)
			current = init_command();
		if (current_token->type == TOKEN_PIPE)
		{
			if (current)
				add_command_to_list(mini, current);
			current = NULL;
		}
		else
			add_token_cmd(current, current_token->type, current_token->value);
		current_token = current_token->next;
	}
	if (current)
		add_command_to_list(mini, current);
	return (0);
}
