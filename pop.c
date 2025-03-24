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
			printf("%s ", cur->content);
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
	while (cur && cur->i == 1)
	{
		printf("%i %s \n", cur->i, cur->line);
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
	cur_a = mini->cmd->args->next;
	if  (!cur_a)
		return ;
	while (cur_a)
	{
		cur = mini->env;
		prev = NULL;
		while (cur)
		{
			if (ft_strncmp(cur->line, cur_a->content,
					ft_strlen(cur_a->content)) == 0)
			{
				printf("la ligne de l env = %s\nla notre = %s\n", cur->line, cur_a->content);
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

	if (mini->cmd->next || !mini)
	return ;
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
				return ;
			if (cur)
			{
				append_node(mini, ft_substr(cur->content, 0,
					ft_strlen(cur->content)), mini->env);
				cur = cur->next;
			}
		}
	}
}

void	export_case_one(t_commandlist *mini)
{
	t_lst	*ev;

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
	int	i;

	i = 0;
	if (ft_isdigit(cur->content[0]) == 1)
		return (printf("bash: export: `%s': not a valid identifier\n", cur->content), 0);
	while (cur->content[i])
	{
		printf("la= %c\n\n\n", cur->content[i]);
		if (ft_isalnum(cur->content[i]) ==  1 || cur->content[i] == '=')
			i++;
		else
			return (printf("bash: export: `%s': not a valid identifier\n", cur->content), 0);
	}
	return (1);

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

	cur = mini->cmd->args;
	if (!cur->next)
	{
		chdir("/home");
		return ;
	}
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:49:31 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/24 13:23:26 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_numlen(int nbr)
{
	int	i;

	i = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		i++;
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		i++;
		nbr /= 10;
	}
	return (i);
}

void	set_env(t_commandlist *mini, char **env)
{
	int		i;
	t_lst	*newnode;
	t_lst	*last;

	i = 0;
	if (!env || !*env)
		exit(0);
	mini->res = 2;
	while (env[i])
	{
		newnode = malloc(sizeof(t_lst));
		if (!newnode)
			return ;
		newnode->line = ft_strdup(env[i]);
		newnode->next = NULL;
		newnode->pre = NULL;
		newnode->i = 1;
		if (!mini->env)
			mini->env = newnode;
		else
		{
			last = find_last(mini->env);
			last->next = newnode;
			newnode->pre = last;
		}
		i++;
	}
}

t_lst	*find_last(t_lst *stack)
{
	if (!stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:51:28 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/19 13:53:46 by ssoukoun         ###   ########.fr       */
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
	int		k;

	exit_char = ft_itoa(mini->res);
	if (!exit_char)
		return ;
	k = 0;
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
	printf("NEWLEN %d\n", new_len);
	expanded = malloc(new_len + 1);
	if (!expanded)
		return (NULL);
	while (content[i])
	{
		if (content[i] == '"' || content[i] == '\'')
		{
			toggle_quote(content[i], &in_double_quote, &in_single_quote);
			i++;
		}
		else if (content[i] == '$' && !in_single_quote)
		{
			i++;
			if (content[i] == '?')
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 12:55:00 by marvin            #+#    #+#             */
/*   Updated: 2025/02/26 12:55:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exe(t_commandlist *mini)
{
	t_command	*cur;
	int			*id;
	int			i;

	cur = mini->cmd;
	i = cmd_len(mini->cmd) + 1;
	id = malloc((sizeof(int) * i));
	i = 0;
	if (!id || !cur)
		return ;
	while (cur)
	{
		cr_pip(cur), id[i] = cr_fork();
		if (id[i] == 0)
		{
			child_pr(cur, mini);
			free(id);
			mini->res = 127;
			ft_exit(mini, NULL);
		}
		close_pip(cur), cur = cur->next;
		i++;
	}
	if (id)
		mini->res = waiting_room(id, i);
}

char	**findpath(t_commandlist *mini)
{
	char	*line;
	char	**paths;
	t_lst	*cur;

	cur = mini->env;
	while (cur)
	{
		if (ft_strnstri(cur->line, "PATH", 4) == 1)
		{
			line = ft_substr(cur->line, 5, ft_strlen(cur->line));
			paths = ft_split(line, ':');
			free(line);
			return (paths);
		}
		cur = cur->next;
	}
	return (NULL);
}

void	child_pr(t_command *cmd, t_commandlist *mini)
{
	char	*cmde;
	char	**paths;
	char	**argss;
	char	*tmp;
	int		i;

	red(cmd, mini);
	paths = findpath(mini);
	i = 0;
	cas(mini);
	argss = arg_to_tab(cmd->args);
	while (paths && paths[++i])
	{
		printf("sa pass pkkk \n\n\n");
		tmp = ft_strjoin(paths[i], "/");
		cmde = ft_strjoin(tmp, argss[0]);
		free(tmp);
		if (access(cmde, X_OK | F_OK) == 0)
			execve(cmde, argss, NULL);
		free(cmde);
	}
	err_cmd(paths, argss);
}

void	cas(t_commandlist *mini)
{
	char	**argss;
	char	**env;

	if (*mini->cmd->args->content == '/')
	{
		env = lst_to_tab(mini->env);
		argss = arg_to_tab(mini->cmd->args);
		if (access(argss[0], X_OK | F_OK) == 0)
			execve(mini->cmd->args->content, argss, env);
		free_tab(argss);
		free_tab(env);
	}
}

void	verif_pipe(t_command *cur)
{
	if (cur->prev)
	{
		dup2(cur->prev->pipe[0], STDIN_FILENO);
		close(cur->prev->pipe[0]);
	}
	if (cur->next)
	{
		dup2(cur->pipe[1], STDOUT_FILENO);
		close(cur->pipe[1]);
	}
}
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
		if (content[i] == '"' || content[i] == '\'')
			toggle_quote(content[i++], &in_double_quote, &in_single_quote);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:24:41 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/19 14:25:39 by ssoukoun         ###   ########.fr       */
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
	if (input)
		free(input);
	if (mini->env)
		free_lst(mini->env);
	if (mini)
		free_shell(mini);
	free(mini);
	rl_clear_history();
	exit(0);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:43:20 by marvin            #+#    #+#             */
/*   Updated: 2025/02/04 17:58:05 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

t_commandlist	*init_shell(void)
{
	t_commandlist	*mini;

	mini = malloc(sizeof(t_commandlist));
	if (!mini)
		return (NULL);
	mini->tokens = NULL;
	mini->cmd = NULL;
	mini->env = NULL;
	mini->res = 0;
	return (mini);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		signal(SIGQUIT, SIG_IGN);
}

void	boucle(t_commandlist *mini)
{
	char	*input;

	while (1)
	{
		input = readline("user:");
		if (!input)
			break ;
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			free(input);
			continue ;
		}
		if (*input)
			add_history(input);
		if (parsing(input, mini) != 0)
		{
			free(input);
			free_shell(mini);
			continue ;
		}
		free(input);
		free_shell(mini);
	}
}

void	init_base(t_commandlist *mini, char **env)
{
	set_env(mini, env);
	signal(SIGQUIT, signal_handler);
	signal(SIGINT, signal_handler);
}

void	clear_env(t_commandlist *mini)
{
	t_lst			*temp_env;
	t_lst			*next_env;

	temp_env = mini->env;
	while (temp_env)
	{
		next_env = temp_env->next;
		if (temp_env->line)
			free(temp_env->line);
		free(temp_env);
		temp_env = next_env;
	}
	mini->env = NULL;

}

int	main(int ac, char **argv, char **env)
{
	t_commandlist	*mini;
	int				i;

	(void)ac;
	(void)argv;
	mini = init_shell();
	if (!mini)
	{
		printf("Error allocating memory\n"), 
		exit(-1);
	}
	init_base(mini, env);
	boucle (mini);
	i = mini->res;
	clear_env(mini);	
	free_shell(mini);
	free(mini);
	rl_clear_history();
	printf("exit\n");
	return (i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:52:32 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/24 14:15:34 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command(void)
{
	t_command	*new_command;

	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->args = NULL;
	new_command->infile = NULL;
	new_command->outfile = NULL;
	new_command->append = NULL;
	new_command->heredoc = NULL;
	new_command->next = NULL;
	new_command->prev = NULL;
	new_command->pipe[0] = -1;
	new_command->pipe[1] = -1;
	if (pipe(new_command->pipe) < 0)
		return (printf("looose\n"), NULL);
	return (new_command);
}

void	add_arg(t_arg **arg_list, char *str)
{
	t_arg	*temp;
	t_arg	*new_arg;

	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return ;
	new_arg->pipe[0] = -1;
	new_arg->pipe[1] = -1;
	if (pipe(new_arg->pipe) < 0)
		return ;
	new_arg->content = strdup(str);
	if (!new_arg->content)
	{
		free(new_arg);
		return ;
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
	t_command	*temp;

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
			break;
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
	if (checking_error_before(input))
		return (1);
	if (lexing(input, mini) == 1)
		return (1);
	if (parse_token(mini) == 1)
		return (1);
	expand_variables(mini);
	build_in(mini, input);
	/*print_args(mini);
	print_commands(mini->cmd);*/
	return (0);
}
void print_args(t_commandlist *mini)
{
	t_token *temp_tokens;
	
	temp_tokens = mini->tokens;
    while (temp_tokens)
    {
        printf("Token: %s, Type: %d\n", temp_tokens->value, temp_tokens->type);
        temp_tokens = temp_tokens->next;
    }
}

void print_redirections(t_arg *redir, const char *type)
{
    while (redir)
    {
        printf("  %s: %s\n", type, redir->content);
        redir = redir->next;
    }
}

void print_commands(t_command *cmd)
{
	t_command	*temp;

	temp = cmd;
    while (temp)
    {
        printf("Commande:\n");
        
        // Affiche les arguments
        t_arg *arg = temp->args;
        while (arg)
        {
            printf("  Argument: %s\n", arg->content);
            arg = arg->next;
        }
        // Affiche les redirections si elles existent
        if (temp->infile)
            print_redirections(temp->infile, "Infile");
        if (temp->outfile)
            print_redirections(temp->outfile, "Outfile");
        if (temp->append)
            print_redirections(temp->append, "Append");
        if (temp->heredoc)
            print_redirections(temp->heredoc, "Heredoc");
        temp = temp->next;
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:04:36 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/24 14:38:49 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	red(t_command *cur, t_commandlist *mini)
{
	verif_pipe(cur);
	her_doc(cur, mini);
	in_file(cur);
	out_file(cur);
	add_file(cur);
}

void	in_file(t_command *cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur->infile;
	while (fil)
	{
		if (access(fil->content, F_OK) == -1)
		{
			printf("zsh: no such file or directory: %s \n", fil->content);
			return ;
		}
		if (access(fil->content, R_OK) == -1)
		{
			printf("zsh: permission denied: %s \n", fil->content);
			return ;
		}
		fd = open(fil->content, O_RDONLY, 0644);
		if (fd < 0)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDIN_FILENO);
		(close(fd), fil = fil->next);
	}
}

void	out_file(t_command *cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur->outfile;
	fd = -1;
	while (fil)
	{
		fd = open(fil->content, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (fd < 0 || access(fil->content, R_OK | W_OK) == -1)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fil = fil->next;
	}
}

void	add_file(t_command *cur)
{
	t_arg	*fil;
	int		fd;

	fil = cur->append;
	fd = -1;
	while (fil)
	{
		fd = open(fil->content, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd < 0 || access(fil->content, R_OK | W_OK) == -1)
		{
			printf("can't open %s \n", fil->content);
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fil = fil->next;
	}
}

void	her_doc(t_command *cur, t_commandlist *mini)
{
	t_arg	*fil;

	fil = cur->heredoc;
	if (!fil)
		return ;
	if (fil->content == NULL || fil->content[0] == '\0')
		ft_exit(mini, NULL);
	her_doc_p2(fil, NULL, mini);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:09:10 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/24 14:38:03 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_cmd(char **tab, char **tabb)
{
	printf("%s: command not found\n", tabb[0]);
	free_tab(tab);
	free_tab(tabb);
	//ft_exit(mini, NULL);
}

void	clean_lst(t_lst *lst)
{
	t_lst	*cur;
	t_lst	*next;

	cur = lst;
	while (cur)
	{
		next = cur->next;
		free(cur->line);
		free(cur);
		cur = next;
	}
	free(lst);
}

int	ft_strnstri(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	a;

	i = 0;
	if (!*needle)
		return (-1);
	while (haystack[i] && i < len)
	{
		a = 0;
		while (haystack[i + a] && needle[a] && haystack[i + a]
			&& len > i + a && haystack[i + a] == needle[a])
			a++;
		if (!needle[a])
			return (1);
		i++;
	}
	return (-1);
}

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab && tab[i])
		free(tab[i++]);
	free(tab);
	tab = NULL;
}

void	her_doc_p2(t_arg *fil, char *line, t_commandlist *mini)
{
    while (fil)
    {
        while (1)
        {
            line = readline("hd:");
            if (!line)
            {
                printf("heredoc: unexpected EOF\n");
                break;
            }
            if (strcmp(line, fil->content) == 0) // Vérifie le délimiteur
            {
                free(line);
                break;
            }
			line = look_variables(line, mini);
            write(fil->pipe[1], line, strlen(line)); // Écrit dans le tube
            write(fil->pipe[1], "\n", 1); // Ajoute un saut de ligne
            free(line);
        }
		dup2(fil->pipe[0], STDIN_FILENO); // Redirige l'entrée standard vers le tube
		close(fil->pipe[1]); // Ferme l'extrémité d'écriture du tube
		close(fil->pipe[0]); // Ferme l'extrémité de lecture du tube
        fil = fil->next;
    }
}

char  *look_variables(char *content, t_commandlist *mini)
{
	char *tmp;

	tmp = expand_env(mini, content);
	if (tmp)
		return (tmp);
	return (content);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:11:14 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/24 14:43:49 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	waiting_room(int *id, int j)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < j--)
	{
		printf("dans le wait \n\n\n");	
		waitpid(id[j], &status, 0);
	}
	free(id);
	return (status);
}

int	cr_fork(void)
{
	int	i;

	i = fork();
	if (i < 0)
	{
		perror("fork");
	}
	return (i);
}

void	close_pip(t_command *cur)
{
	if (cur->prev)
		close(cur->prev->pipe[0]);
	if (cur->next)
		close(cur->pipe[1]);
}

void	cr_pip(t_command *cur)
{
	if (cur->next)
	{
		if (pipe(cur->pipe) == -1)
		{
			perror("pipe");
			return ;
		}
	}
}

int	lefork(void)
{
	int	i;

	i = fork();
	if (i < 0)
		return (perror("fork1"), -1);
	return (i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:06:14 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/19 00:22:54 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_len(t_command *cmd)
{
	int			i;
	t_command	*cur;

	cur = cmd;
	i = 0;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

int	list_len(t_lst *lst)
{
	int		i;
	t_lst	*cur;

	i = 0;
	cur = lst;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

char	**lst_to_tab(t_lst *lst)
{
	int		i;
	int		len;
	char	**tab;
	t_lst	*cur;

	if (!lst || !lst)
		return (NULL);
	len = list_len(lst);
	cur = lst;
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (cur)
	{
		tab[i] = ft_strdup(cur->line);
		cur = cur->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

char	**arg_to_tab(t_arg *lst)
{
	int		i;
	char	**tab;
	t_arg	*cur;

	i = arg_len(&lst);
	tab = malloc(sizeof(char *) * (i + 1));
	i = 0;
	cur = lst;
	if (!tab)
		return (NULL);
	while (cur)
	{
		tab[i] = ft_strdup(cur->content);
		cur = cur->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

int	arg_len(t_arg **ar)
{
	int		i;
	t_arg	*cur;

	i = 0;
	cur = *ar;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:20:05 by ssoukoun          #+#    #+#             */
/*   Updated: 2025/03/23 19:22:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2, int j)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && i < j)
		i++;
	if (s1[i] != s2[i])
	{
		return (-1);
	}
	return (0);
}

void	redirect_lines(t_lst *lst)
{
	int		pipes[2];
	t_lst	*act;

	act = lst;
	if (pipe(pipes) < 0)
		return ;
	while (act)
	{
		write(pipes[1], act->line, ft_strlen(act->line));
		write(pipes[1], "\n", 1);
		act = act->next;
	}
	close(pipes[1]);
	dup2(pipes[0], STDIN_FILENO);
	close(pipes[0]);
	clean_lst(lst);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:51:41 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/19 14:16:08 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirection_syntax(char *input, int *i)
{
	char	temp;

	temp = input[*i];
	(*i)++;
	if (input[*i] == temp)
		(*i)++;
	while (isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\0' || input[*i] == '>'
		|| input[*i] == '<' || input[*i] == '|')
	{
		printf("Syntax error near unexpected token\n");
		return (1);
	}
	return (0);
}

int	check_pipe_syntax(char *input, int *i)
{
	(*i)++;
	while (isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\0' || input[*i] == '>'
		|| input[*i] == '<' || input[*i] == '|')
	{
		printf("Syntax error near unexpected token\n");
		return (1);
	}
	return (0);
}

int	checking_syntax(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '|' || (input[i] == '<' && input[i + 1] != '<')
		|| input[i] == '>')
	{
		printf("Syntax error near unexpected token\n");
		return (1);
	}
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<')
		{
			if (check_redirection_syntax(input, &i))
				return (1);
		}
		else if (input[i] == '|')
		{
			if (check_pipe_syntax(input, &i))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

int	checking_error_before(char *input)
{
	return (only_space(input) || open_quote(input) || checking_syntax(input));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:54:35 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/24 13:23:51 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_single_redirect(char *input, int *i, t_commandlist *mini, t_token_type token_type)
{
	int		start;
	char	*file;

	(*i) += 1;
	while (input[*i] && isspace(input[*i]))
		(*i)++;
	start = *i;
	while (input[*i] && !is_special(input[*i]) && !isspace(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (skip_quotes(input, i))
				return;
		}
		else
			(*i)++;
		if (input[*i] == '\0')
			break;
	}
	if (start == *i)
		return;
	file = ft_strndup(&input[start], *i - start);
	add_token(mini, token_type, file);
	free (file);
	while (input[*i] && isspace(input[*i]))
		(*i)++;
}

void handle_pipe(char *input, int *i, t_commandlist *mini, t_token_type token_type)
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
	printf("Création du token -> Adresse : %p, Valeur : %p (%s), Type : %d\n", (void *)new_token, (void *)new_token->value, new_token->value, new_token->type);
	return (0);
}

void handle_double_redirect(char *input, int *i, t_commandlist *mini, t_token_type token_type)
{
	int		start;
	char	*file;

	(*i) += 2;
	while (input[*i] && isspace(input[*i]))
		(*i)++;
	start = *i;
	while (input[*i] && !is_special(input[*i]) && !isspace(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (skip_quotes(input, i))
				return;
		}
		else
			(*i)++;
		if (input[*i] == '\0')
			break;
	}
	file = ft_strndup(&input[start], *i - start);
	add_token(mini, token_type, file);
	free (file);
	while (input[*i] && isspace(input[*i]))
		(*i)++;
}

void	append_node(t_commandlist *mini, char *env, t_lst *lst)
{
	t_lst	*newnode;
	t_lst	*last;
	(void)mini;

	if (!lst) // Vérification de lst avant d'accéder à *lst
		return ;

	newnode = malloc(sizeof(t_lst));
	if (newnode == NULL)
		return ;
	newnode->next = NULL;
	newnode->i = 0;
	newnode->line = ft_strdup(env);
	newnode->pre = NULL;

	if (lst == NULL) // Si la liste est vide
	{
		lst = newnode;
	}
	else
	{
		last = find_last(lst);
		if (last) // Vérifier que find_last a bien trouvé un dernier élément
		{
			last->next = newnode;
			newnode->pre = last;
		}
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoukoun <ssoukoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:57:40 by fsingh            #+#    #+#             */
/*   Updated: 2025/02/04 19:13:46 by ssoukoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ispace(char c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\f' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int is_special(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int only_space(char *input)
{
	int i;

	i = 0;
	if (!input || input[0] == '\0')
	{
		//printf("\n");
		return (1);
	}
	while (input[i])
	{
		if (!ispace(input[i]))
			return (0);
		i++;
	}
	printf("\n");
	return (1);
}

char *ft_strndup(const char *str, size_t n)
{
	char *res;
	size_t i;

	i = 0;
	res = (char *)malloc(n + 1);
	if (res == NULL)
		return (NULL);
	while (i < n && str[i] != '\0')
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int open_quote(char *input)
{
	int i;
	char quote;

	i = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (1);
		}
		i++;
	}
	return (0);
}
