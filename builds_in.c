/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulds_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 12:58:45 by marvin            #+#    #+#             */
/*   Updated: 2025/02/25 12:58:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_in(t_commandlist *mini, char *input)
{
	char	*str;

	str = mini->cmd->args->content;
	if (ft_strcmp(str, "exit", ft_strlen("exit")) == 0)
		ft_exit(mini, input);
	else if (ft_strcmp(str, "echo", ft_strlen("echo")) == 0)
		ft_echo(mini);
	else if (ft_strcmp(str, "env", ft_strlen("env")) == 0)
		ft_env(mini);
	else if (ft_strcmp(str, "unset", ft_strlen("unset")) == 0)
		ft_unset(mini);
	else if (ft_strcmp(str, "export", ft_strlen("export")) == 0)
		ft_export(mini);
	else if (ft_strcmp(str, "pwd", ft_strlen("pwd")) == 0)
		ft_pwd(mini);
	else if (ft_strcmp(str, "cd", ft_strlen("cd")) == 0)
		ft_cd(mini);
	else
		exe(mini);
}

void	ft_exit(t_commandlist *mini, char *input)
{
	int		i;
	t_arg	*cur;

	cur = mini->cmd->args->next;
	if (!cur)
		clean_up_and_exit(input, mini);
	if (cur->next)
	{
		printf("trop d arg\n\n");
		return ;
	}
	else
	{
		if (isnum(cur->content) == 0)
		{
			i = ft_atoi(cur->content);
			mini->res = i;
			clean_up_and_exit(input, mini);
		}
		else
			clean_up_and_exit(input, mini);
	}
	return ;
}

void	ft_echo(t_commandlist *mini)
{
	t_arg	*cur;

	if (mini->cmd->next || !mini->cmd->args->next || mini->cmd->outfile
		|| mini->cmd->infile || mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return ;
	}
	cur = mini->cmd->args->next;
	if (ft_strncmp(cur->content, "-n", 3) == 0)
	{
		while (cur && cur->next)
		{
			cur = cur->next;
			printf("%s", cur->content);
		}
	}
	else
	{
		while (cur)
		{
			printf("%s", cur->content);
			cur = cur->next;
		}
		printf("\n");
	}
}

void	ft_env(t_commandlist *mini)
{
	t_lst	*cur;

	if (mini->cmd->next || mini->cmd->outfile
		|| mini->cmd->infile || mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return ;
	}
	cur = (mini->env);
	while (cur)
	{
		printf("%s \n", cur->line);
		cur = cur->next;
	}
}

void	ft_unset(t_commandlist *mini)
{
	t_lst	*cur;
	t_arg	*cur_a;
	t_lst	*del;
	t_lst	*prev;

	if (mini->cmd->next)
		return ;
	cur_a = mini->cmd->args;
	while (cur_a)
	{
		cur = mini->env;
		prev = NULL;
		while (cur)
		{
			if (ft_strncmp(cur->line, cur_a->content,
					ft_strlen(cur_a->content)) == 0)
			{
				del = cur;
				cur = cur->next;
				if (del == mini->env)
					mini->env = cur;
				if (prev)
				{
					prev->next = cur;
					if (cur)
						cur->pre = prev;
				}
				printf("jsp\n");
				deletenode(del);
				break ;
			}
			cur = cur->next;
		}
		prev = cur;
	}
	cur_a = cur_a->next;
}

void	deletenode(t_lst *out)
{
	if (!out)
	{
		printf("Erreur : nœud invalide\n");
		return ;
	}
	if (out->pre)
	{
		if (out->next)
		{
			out->pre->next = out->next;
			out->next->pre = out->pre;
		}
		else
			out->pre->next = NULL;
	}
	else if (out->next)
		out->next->pre = NULL;
	out->pre = NULL;
	out->next = NULL;
}

int	ft_stri(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	if (str[i])
		return (i);
	return (-1);
}

void	ft_export(t_commandlist *mini)
{
	t_arg	*cur;
	t_lst	*ev;

	if (mini->cmd->next || !mini)
		return ;
	cur = mini->cmd->args;
	if (!cur->next)
	{
		ev = mini->env;
		while (ev)
		{
			printf("export ");
			printf("%s\n", ev->line);
			ev = ev->next;
		}
	}
	else
	{
		cur = cur->next;
		while (cur)
		{
			if (ft_stri(cur->content, '=') == -1)
				return ;
			append_node(mini, ft_substr(cur->content, 0,
					ft_strlen(cur->content)), mini->env);
			cur = cur->next;
		}
	}
}

void	ft_pwd(t_commandlist *mini)
{
	if (mini->cmd->next || !mini->cmd->args->next || mini->cmd->outfile
		|| mini->cmd->infile || mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return ;
	}
	printf("%s\n", getcwd(NULL, 100));
}

void	ft_cd(t_commandlist *mini)
{
	t_arg	*cur;

	cur = mini->cmd->args->next;
	if (cur->next)
	{
		printf("too many arguments\n");
		return ;
	}
	if (mini->cmd->next || !mini->cmd->args->next || mini->cmd->outfile
		|| mini->cmd->infile || mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return ;
	}
	else if (chdir(cur->content) == -1)
	{
		printf("directory problem\n");
		return ;
	}
}

int	isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}
