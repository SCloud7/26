/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:51:28 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/04/01 04:28:25 by fsingh           ###   ########.fr       */
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
		if (!new_content)
			new_content = ft_strdup("");
		free(arg->content);
		arg->content = new_content;
		arg = arg->next;
	}
}

void	handle_env_variable(t_commandlist *mini, char *content,
								char *expanded, t_expand *exp)
{
	int		start;
	int		var_len;
	char	*var_name;
	char	*env_temp;
	int		k;

	start = exp->i;
	while (content[exp->i] && (ft_isalnum(content[exp->i])
			||content[exp->i] == '_'))
		exp->i++;
	var_len = exp->i - start;
	var_name = ft_substr(content, start, var_len);
	if (!var_name)
		return ;
	env_temp = search_env(mini->env, var_name);
	free(var_name);
	if (env_temp)
	{
		k = 0;
		while (env_temp[k])
			expanded[exp->j++] = env_temp[k++];
	}
}

void	expand_loop(t_commandlist *mini, char *content,
	t_expand *exp, char *expanded)
{
	while (content[exp->i] && exp->i < ft_strlen(content))
	{
		if (content[exp->i] == '"' || content[exp->i] == '\'')
			handle_quotes(expanded, content, exp);
		else if (content[exp->i] == '$' && !exp->in_single_quote)
		{
			exp->i++;
			if (content[exp->i] == '\0' || isspace(content[exp->i])
				|| is_at_end(content, exp->i))
				expanded[exp->j++] = '$';
			else if (content[exp->i] == '?')
			{
				handle_exit_status(mini, expanded, &exp->j);
				exp->i++;
			}
			else if (ft_isalnum(content[exp->i]) || content[exp->i] == '_')
				handle_env_variable(mini, content, expanded, exp);
			else
				continue ;
		}
		else
			expanded[exp->j++] = content[exp->i++];
	}
}

char	*expand_env(t_commandlist *mini, char *content)
{
	t_expand	exp;
	char		*expanded;
	int			new_len;

	exp = (t_expand){0, 0, 0, 0};
	if (!content)
		return (NULL);
	new_len = calc_expand_length(mini, content);
	expanded = malloc(new_len + 1);
	if (!expanded)
		return (NULL);
	expand_loop(mini, content, &exp, expanded);
	expanded[exp.j] = '\0';
	return (expanded);
}
