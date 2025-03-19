#include "minishell.h"

void free_args(t_arg *args)
{
    t_arg *tmp;

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

void free_command(t_command *cmd)
{
    t_command   *tmp;

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

void free_shell(t_commandlist *mini)
{
    t_token     *temp;
    t_token     *next_token;
    t_command   *temp_command;

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

void    clean_up_and_exit(char *input, t_commandlist *mini)
{
    if (input)
        free(input);
    if (mini)
        free_shell(mini);
    free(mini);
    rl_clear_history();
    exit(0);
}

void    free_env(t_commandlist *mini)
{
    t_lst *next_env;
    t_lst *temp_env = mini->env;
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


void    free_lst(t_lst *lst)
{
    t_lst *cur;

    cur = lst;
    while (cur)
    {
        free(cur->line);
        cur->line = NULL;
//        free(cur);
        cur = cur->next;
    }
    //free(lst);
}
