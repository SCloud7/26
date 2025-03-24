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
