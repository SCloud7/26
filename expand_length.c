/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_length.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:51:20 by fsingh            #+#    #+#             */
/*   Updated: 2025/04/01 04:39:28 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*search_env(t_lst *env, char *key)
{
	int	key_len;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->line, key, key_len) == 0
			&& env->line[key_len] == '=')
			return (env->line + key_len + 1);
		env = env->next;
	}
	return (NULL);
}

int	get_env_length(t_commandlist *mini, char *var)
{
	char	*value;

	value = search_env(mini->env, var);
	if (value)
		return (ft_strlen(value));
	return (0);
}

int	get_var_lenght(t_commandlist *mini, char *content, int *i)
{
	int		start;
	int		var_len;
	char	*var_name;
	int		len;

	start = *i;
	while (content[*i] && (ft_isalnum(content[*i]) || content[*i] == '_'))
		(*i)++;
	var_len = *i - start;
	var_name = ft_substr(content, start, var_len);
	len = get_env_length(mini, var_name);
	free(var_name);
	return (len);
}

void	quote_lenght(t_expand_lenght *exp_l, char *content)
{
	if (content[exp_l->i] == '"' && !exp_l->in_single_quote)
	{
		exp_l->in_double_quote = !exp_l->in_double_quote;
		exp_l->i++;
	}
	else if (content[exp_l->i] == '\'' && !exp_l->in_double_quote)
	{
		exp_l->in_single_quote = !exp_l->in_single_quote;
		exp_l->i++;
	}
	else if ((content[exp_l->i] == '"' && exp_l->in_single_quote)
		|| (content[exp_l->i] == '\'' && exp_l->in_double_quote))
	{
		exp_l->new_len++;
		exp_l->i++;
	}
}

int	calc_expand_length(t_commandlist *mini, char *content)
{
	t_expand_lenght	exp_l;

	exp_l = (t_expand_lenght){0, 0, 0, 0};
	while (content[exp_l.i])
	{
		if (content[exp_l.i] == '\'' || content[exp_l.i] == '"')
			quote_lenght(&exp_l, content);
		else if (content[exp_l.i] == '$' && !exp_l.in_single_quote)
		{
			exp_l.i++;
			if (content[exp_l.i] == '\0' || isspace(content[exp_l.i]))
				exp_l.new_len++;
			else if (content[exp_l.i] == '?')
				expand_exit_lenght(mini, &exp_l);
			else if (ft_isalnum(content[exp_l.i]) || content[exp_l.i] == '_')
				exp_l.new_len += get_var_lenght(mini, content, &exp_l.i);
			continue ;
		}
		else
		{
			exp_l.new_len++;
			exp_l.i++;
		}
	}
	return (exp_l.new_len);
}
