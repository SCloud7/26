/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:51:28 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/26 17:01:45 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variables(t_commandlist *mini)
{
	t_command	*cmd;

	cmd = mini->cmd;
	while (cmd)
	{
		expand_args(mini, cmd->args);
		expand_args(mini, cmd->infile);
		expand_args(mini, cmd->outfile);
		expand_args(mini, cmd->append);
		cmd = cmd->next;
	}
}

void	expand_args(t_commandlist *mini, t_arg *to_expand)
{
	t_arg	*arg;
	char	*new_content;

	arg = to_expand;
	while (arg)
	{
		new_content = expand_env(mini, arg->content);
		free(arg->content);
		arg->content = new_content;
		arg = arg->next;
	}
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

void	handle_env_variable(t_commandlist *mini, char *content, char *expanded, int *i, int *j)
{
	int		start;
	int		var_len;
	char	*var_name;
	char	*env_temp;
	int		k;

	start = *i;
	while (content[*i] && (ft_isalnum(content[*i])
			||content[*i] == '_'))
		(*i)++;
	var_len = *i - start;
	var_name = ft_substr(content, start, var_len);
	if (!var_name)
		return ;
	env_temp = search_env(mini->env, var_name);
	free(var_name);
	if (env_temp)
	{
		k = 0;
		while (env_temp[k])
			expanded[(*j)++] = env_temp[k++];
	}
}

char	*expand_env(t_commandlist *mini, char *content)
{
	int		i;
	int		j;
	int		in_double_quote;
	int		in_single_quote;
	char	*expanded;
	int		new_len;

	j = 0;
	i = 0;
	in_double_quote = 0;
	in_single_quote = 0;
	if (!content)
		return (NULL);
	new_len = calc_expand_length(mini, content);
	expanded = malloc(new_len + 1);
	if (!expanded)
		return (NULL);
	while (content[i])
	{
		if (content[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
		}
		else if (content[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
		}
		else if ((content[i] == '"' && in_single_quote) || (content[i] == '\'' && in_double_quote))
			expanded[j++] = content[i++];
		else if (content[i] == '$' && !in_single_quote)
		{
			i++;
			if (content[i] == '\0' || isspace(content[i]))
				expanded[j++] = '$';
			else if (content[i] == '?')
			{
				handle_exit_status(mini, expanded, &j);
				i++;
			}
			else if (ft_isalnum(content[i]) || content[i] == '_')
				handle_env_variable(mini, content, expanded, &i, &j);
			continue ;
		}
		else
			expanded[j++] = content[i++];
	}
	expanded[j] = '\0';
	return (expanded);
}
