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
			mini->res = 127;
			ft_exit(mini, NULL);
		}
		close_pip(cur), cur = cur->next;
		i++;
	}
	mini->res = waiting_room(id, i);
	free(id);
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
		tmp = ft_strjoin(paths[i], "/");
		cmde = ft_strjoin(tmp, argss[0]);
		free(tmp);
		if (access(cmde, X_OK | F_OK) == 0)
			execve(cmde, argss, NULL);
		free(cmde);
	}
	err_cmd(mini, paths, argss);
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
