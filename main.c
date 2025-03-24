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

	if (isatty(STDIN_FILENO) == 0 || isatty(STDOUT_FILENO) == 0)
		exit(-1);
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
