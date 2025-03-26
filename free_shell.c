/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:24:41 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/26 20:45:01 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(t_arg *args)
{
	t_arg	*tmp;

	tmp = args;
	while (tmp)
	{
		tmp = args->next;
		if (args->content)
		{
			free(args->content);
			args->content = NULL;
		}
		free(args);
		args = tmp;
	}
}

void	free_command(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
			free_args(cmd->args);
		if (cmd->infile)
			free_args(cmd->infile);
		if (cmd->outfile)
			free_args(cmd->outfile);
		if (cmd->append)
			free_args(cmd->append);
		if (cmd->heredoc)
			free_args(cmd->heredoc);
		free(cmd);
		cmd = tmp;
	}
}

void	free_shell(t_commandlist *mini)
{
	t_token		*temp;
	t_token		*next_token;
	t_command	*temp_command;

	if (mini)
	{
		temp = mini->tokens;
		while (temp)
		{
			next_token = temp->next;
			{
				free(temp->value);
				temp->value = NULL;
			}
			free(temp);
			temp = next_token;
		}
	}
	temp_command = mini->cmd;
	if (temp_command)
	{
		free_command(temp_command);
		mini->cmd = NULL;
	}
	mini->tokens = NULL;
}

void	clean_up_and_exit(char *input, t_commandlist *mini)
{
	int	i;

	i = mini->res;
	if (input)
		free(input);
	if (mini->env)
		free_lst(mini->env);
	if (mini)
		free_shell(mini);
	free(mini);
	rl_clear_history();
	exit(i);
}

void	free_lst(t_lst *lst)
{
	t_lst	*cur;
	t_lst	*tmp;

	cur = lst;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->line);
		free(tmp);
	}
}
