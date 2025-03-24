/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_length.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:51:20 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/19 14:27:21 by ssoukoun         ###   ########.fr       */
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

void	toggle_quote(char c, int *in_double_quote, int *in_single_quote)
{
	if (c == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
	else if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
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

int	calc_expand_length(t_commandlist *mini, char *content)
{
	int		i;
	int		new_len;
	int		in_double_quote;
	int		in_single_quote;

	i = 0;
	new_len = 0;
	in_double_quote = 0;
	in_single_quote = 0;
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
		{
			new_len++;
			i++;
		}
		else if (content[i] == '$' && !in_single_quote)
		{
			i++;
			if (content[i] == '?')
			{
				new_len += ft_numlen(mini->res);
				i++;
			}
			else if (ft_isalnum(content[i]) || content[i] == '_')
				new_len += get_var_lenght(mini, content, &i);
			continue ;
		}
		else
		{
			new_len++;
			i++;
		}
	}
	return (new_len);
}
