/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsingh <fsingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 01:39:09 by fsingh            #+#    #+#             */
/*   Updated: 2025/03/28 01:39:09 by fsingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_expand
{
	int	i;
	int	j;
	int	in_double_quote;
	int	in_single_quote;
}	t_expand;

typedef struct s_expand_lenght
{
	int	i;
	int	new_len;
	int	in_double_quote;
	int	in_single_quote;
}	t_expand_lenght;

typedef struct s_lst
{
	char			*line;
	int				i;
	struct s_lst	*next;
	struct s_lst	*pre;
}				t_lst;

typedef enum e_token_type
{
	TOKEN_ARG,
	TOKEN_PIPE,
	TOKEN_OUT,
	TOKEN_IN,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}				t_token;

typedef struct s_arg
{
	char			*content;
	int				pipe[2];
	t_lst			*doc;
	struct s_arg	*next;
}				t_arg;

typedef struct s_command
{
	t_arg				*args;
	t_arg				*infile;
	t_arg				*outfile;
	t_arg				*append;
	int					pipe[2];
	t_arg				*heredoc;
	struct s_command	*prev;
	struct s_command	*next;
}				t_command;

typedef struct s_commandlist
{
	t_token		*tokens;
	t_lst		*env;
	t_command	*cmd;
	int			res;
}				t_commandlist;

void		print_commands(t_command *cmd);
void		print_args(t_commandlist *mini);

// Parsing
int			skip_quotes(char *input, int *i);
int			handle_arguments(char *input, int *i, t_commandlist *mini);
void		handle_special_token(char *input, int *i, t_commandlist *mini);
int			lexing(char *input, t_commandlist *mini);
int			parsing(char *input, t_commandlist *mini);

// Parsing2
t_command	*init_command(void);
void		add_arg(t_arg **arg_list, char *str);
void		add_token_cmd(t_command *current, t_token_type token_type,
				char *str);
void		add_command_to_list(t_commandlist *mini, t_command *current);
int			parse_token(t_commandlist *mini);

// Utils_Parsing_1
int			ispace(char c);
int			is_special(char c);
int			only_space(char *input);
char		*ft_strndup(const char *s, size_t n);
int			open_quote(char *input);

// utils_parsing_2
int			check_redirection_syntax(char *input, int *i);
int			check_pipe_syntax(char *input, int *i);
int			checking_syntax(char *input);
int			checking_error_before(t_commandlist *mini, char *input);

// utils_parsing_3
void		handle_single_redirect(char *input, int *i, t_commandlist *mini,
				t_token_type token_type);
void		handle_pipe(char *input, int *i, t_commandlist *mini,
				t_token_type token_type);
int			add_token(t_commandlist *mini, t_token_type type,
				char *token_value);
void		handle_double_redirect(char *input, int *i, t_commandlist *mini,
				t_token_type token_type);
void		skip_quotes_parse(char *input, int *i);
void		skip_whitespace(char *input, int *i);

// env_init
int			ft_numlen(int nbr);
void		set_env(t_commandlist *mini, char **env);
t_lst		*find_last(t_lst *stack);
void		expand_exit_lenght(t_commandlist *mini, t_expand_lenght *exp_l);
void		toggle_quote(char c, int *in_double_quote, int *in_single_quote);

// env_lenght
char		*search_env(t_lst *env, char *key);
int			get_env_length(t_commandlist *mini, char *var);
void		toggle_quote(char c, int *in_double_quote, int *in_single_quote);
int			get_var_lenght(t_commandlist *mini, char *content, int *i);
int			calc_expand_length(t_commandlist *mini, char *content);

// env_management
void		expand_variables(t_commandlist *mini);
void		expand_args(t_commandlist *mini, t_arg *to_expand);
void		handle_env_variable(t_commandlist *mini, char *content,
				char *expanded, t_expand *exp);
void		expand_loop(t_commandlist *mini, char *content,
				t_expand *exp, char *expanded);
char		*expand_env(t_commandlist *mini, char *content);

// expand_utils
void		append_node(t_commandlist *mini, char *env, t_lst *lst);
int			is_at_end(char *content, int i);
void		handle_exit_status(t_commandlist *mini, char *expanded, int *j);
void		handle_quotes(char *expanded, char *content, t_expand *exp);
void		clear_env(t_commandlist *mini);

// free
void		free_args(t_arg *args);
void		free_command(t_command *cmd);
void		free_shell(t_commandlist *mini);
void		clean_up_and_exit(char *input, t_commandlist *mini);

// build_in
void		build_in(t_commandlist *mini, char *input);
void		ft_exit(t_commandlist *mini, char *input);
int			ft_echo(t_commandlist *mini);
int			ft_env(t_commandlist *mini);
int			ft_unset(t_commandlist *mini);
int			ft_export(t_commandlist *mini);
int			ft_pwd(t_commandlist *mini);
int			ft_cd(t_commandlist *mini);
int			export_case_two(t_arg *cur);
void		export_case_one(t_commandlist *mini);
t_arg		*n_case(t_arg *cur);

// utils_build
void		deletenode(t_lst *out);
int			ft_stri(char *str, char c);
int			isnum(char *str);

// exe
void		exe(t_commandlist *mini);
char		**findpath(t_commandlist *mini);
void		child_pr(t_command *cmd, t_commandlist *mini);
char		**lst_to_tab(t_lst *lst);
void		cas(t_commandlist *mini);
int			list_len(t_lst *lst);
int			lefork(void);
void		red(t_command *cur, t_commandlist *mini);
void		in_file(t_command *cur);
void		out_file(t_command *cur);
void		add_file(t_command *cur);
void		her_doc(t_command *cur, t_commandlist *mini);
void		her_doc_p2(t_arg *fil);
void		redirect_lines(t_lst *lst);
void		free_tab(char **tab);
int			ft_strnstri(const char *haystack, const char *needle, size_t len);
void		clean_lst(t_lst *lst);
char		**arg_to_tab(t_arg *lst);
int			cmd_len(t_command *cmd);
int			arg_len(t_arg **ar);
// utils exe
void		cr_pip(t_command *cur);
int			cmd_len(t_command *cmd);
void		verif_pipe(t_command *cur);
int			waiting_room(int *id, int j);
int			cr_fork(void);
void		err_cmd(t_commandlist *mini, char **tab, char **tabb);
void		close_pip(t_command *cur);
void		free_lst(t_lst *lst);
int			ft_strcmp(char *s1, char *s2, int j);
char		*look_variables(char *content, t_commandlist *mini);

#endif
