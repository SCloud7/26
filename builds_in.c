/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulds_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/02/25 12:58:45 by marvin            #+#    #+#             */
/*   Updated: 2025/02/25 12:58:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_in(t_commandlist *mini, char *input)
{
	char	*str;
	int		status;

	status = 0;
	mini->res = 0;
	str = mini->cmd->args->content;
	if (ft_strcmp(str, "exit", ft_strlen("exit")) == 0)
		ft_exit(mini, input);
	else if (ft_strcmp(str, "echo", ft_strlen("echo")) == 0)
		status = ft_echo(mini);
	else if (ft_strcmp(str, "env", ft_strlen("env")) == 0)
		status = ft_env(mini);
	else if (ft_strcmp(str, "unset", ft_strlen("unset")) == 0)
		status = ft_unset(mini);
	else if (ft_strcmp(str, "export", ft_strlen("export")) == 0)
		status = ft_export(mini);
	else if (ft_strcmp(str, "pwd", ft_strlen("pwd")) == 0)
		status = ft_pwd(mini);
	else if (ft_strcmp(str, "cd", ft_strlen("cd")) == 0)
		status = ft_cd(mini);
	else
		return (exe(mini));
	if (mini->res == 0 || mini->res == status)
		mini->res = status;
}

void	ft_exit(t_commandlist *mini, char *input)
{
	t_arg *cur;

	cur = mini->cmd->args->next;
	if (!cur)
		clean_up_and_exit(input, mini);
	if (cur->next)
	{
		perror("too many arguments\n");
		return ;
	}
	else
		clean_up_and_exit(input, mini);
	return ;
}

void	print_echo(t_arg *cur)
{
	while (cur)
	{
		printf("%s", cur->content);
		if (cur->next)
			printf(" ");
		cur = cur->next;
	}
}

int		ft_echo(t_commandlist *mini)
{
	t_arg *cur;
	int	i;

	i = 1;
	if (mini->cmd->next || !mini->cmd->args->next || mini->cmd->outfile
		|| mini->cmd->infile || mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return (0);
	}
	cur = mini->cmd->args->next;
	while (cur->content[i] == 'n' && cur->content[0] == '-')
	{
		while (cur->content[i] == 'n')
		{
			i++;
			if (cur->content[i] == '\0')
				cur = cur->next;
		}
		print_echo(cur);
		return (0);
	}
	print_echo(cur);
	printf("\n");
	return (0);
}

int		ft_env(t_commandlist *mini)
{
	t_lst *cur;

	if (mini->cmd->next || mini->cmd->outfile || mini->cmd->infile
		|| mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return (0);
	}
	cur = (mini->env);
	while (cur && cur->i == 1)
	{
		printf("%i %s \n", cur->i, cur->line);
		cur = cur->next;
	}
	return (0);
}

int		ft_unset(t_commandlist *mini)
{
	t_lst *cur;
	t_arg *cur_a;
	t_lst *del;
	t_lst *prev;

	if (mini->cmd->next)
		return (0);
	cur_a = mini->cmd->args->next;
	if (!cur_a)
		return (0);
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
				deletenode(del);
				break ;
			}
			cur = cur->next;
			prev = cur;
		}
		cur_a = cur_a->next;
	}
	return (0);
}

t_arg	*n_case(t_arg *cur)
{
	int i;
	t_arg *cc;

	i = 1;
	cc =cur;
	while (cc)
	{
		while (cc->content[i] == 'n' && cc->content[0] == '-')
			i++;
		if (cc->content[i] == '\0')
			cc = cc->next;
	}
	return (cc);
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
	int i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	if (str[i])
		return (i);
	return (-1);
}

int	ft_export(t_commandlist *mini)
{
	t_arg *cur;

	if (mini->cmd->next || !mini)
		return (0);
	cur = mini->cmd->args;
	if (!cur->next)
		export_case_one(mini);
	else
	{
		cur = cur->next;
		while (cur)
		{
			while (cur && export_case_two(cur) == 0)
				cur = cur->next;
			if (cur && ft_stri(cur->content, '=') == -1)
				return (0);
			if (cur)
			{
				append_node(mini, ft_substr(cur->content, 0,
						ft_strlen(cur->content)), mini->env);
				cur = cur->next;
			}
		}
	}
	return (0);
}

void	export_case_one(t_commandlist *mini)
{
	t_lst *ev;

	ev = mini->env;
	while (ev)
	{
		printf("export ");
		printf("%s\n", ev->line);
		ev = ev->next;
	}
}

int	export_case_two(t_arg *cur)
{
	int i;

	i = 0;
	if (ft_isdigit(cur->content[0]) == 1)
		return (printf("bash: export: `%s': not a valid identifier\n",
				cur->content), 0);
	while (cur->content[i])
	{
		if (ft_isalnum(cur->content[i]) == 1 || cur->content[i] == '=')
			i++;
		else
			return (printf("bash: export: `%s': not a valid identifier\n",
					cur->content), 0);
	}
	return (1);
}
int	ft_pwd(t_commandlist *mini)
{
	if (mini->cmd->next || !mini->cmd->args->next || mini->cmd->outfile
		|| mini->cmd->infile || mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return (0);
	}
	printf("%s\n", getcwd(NULL, 100));
	return (0);
}

int	ft_cd(t_commandlist *mini)
{
	t_arg *cur;

	cur = mini->cmd->args;
	if (!cur->next)
	{
		chdir("/home");
		return (0);
	}
	cur = mini->cmd->args->next;
	if (cur->next)
	{
		printf("too many arguments\n");
		return (0);
	}
	if (mini->cmd->next || !mini->cmd->args->next || mini->cmd->outfile
		|| mini->cmd->infile || mini->cmd->append || mini->cmd->heredoc)
	{
		exe(mini);
		return (0);
	}
	else if (chdir(cur->content) == -1)
	{
		printf("directory problem\n");
		return (0);
	}
	return (0);
}

int	isnum(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}
