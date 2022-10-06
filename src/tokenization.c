/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:28:38 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/05 23:12:33 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	convert_for_linked_list(char **str, t_token **tokens)
{
	int	count;
	int	index;
	int	size;

	index = -1;
	while (str[++index])
	{
		count = 0;
		while (str[index][count])
		{
			if (str[index][count] == 7)
				str[index][count] = 32;
			count++;
		}
		size = ft_strlen(str[index]);
		if (index == 0 && ft_strncmp(str[index], "''", size)
			&& ft_strncmp(str[index], "\"\"", size))
			tokens[0] = new_token(str[index]);
		else if (ft_strncmp(str[index], "''", size)
			&& ft_strncmp(str[index], "\"\"", size))
			add_node_last_token(tokens, str[index]);
	}
}

static t_token	*build_tokens(void)
{
	char	**tmp;
	t_token	*tokens;
	t_token	*aux;

	tokens = NULL;
	tmp = ft_split(g_core_var->buff, 32);
	convert_for_linked_list(tmp, &tokens);
	aux = tokens;
	while (aux != NULL)
	{
		aux->quotes = has_quotes(aux->str);
		resolve_string(&aux);
		aux = aux->next;
	}
	free_double(tmp);
	return (tokens);
}

static void	multiplex_fuction(t_token *tokens, int cicle)
{
	if (cicle == 0)
		normalize_token(tokens);
	if (cicle == 1)
		classify_token(tokens);
}

int	size_token(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

void	delete_null_tokens(t_token **tokens)
{
	t_token	*aux_1;
	t_token	*aux_2;

	aux_1 = tokens[0];
	if (tokens[0]->str[0] == '\0' && size_token(tokens[0]) == 1)
	{
		free(tokens[0]->str);
		tokens[0]->str = NULL;
		free(tokens[0]);
		tokens[0] = NULL;
		return ;
	}
	while (aux_1)
	{
		aux_2 = NULL;
		if (aux_1->str[0] == '\0')
		{
			if (aux_1->previus)
				aux_2 = aux_1->previus;
			else
			{
				aux_2 = aux_1->next;
				aux_2->previus = NULL;
				tokens[0] = aux_2;
			}
			if (aux_1->previus && aux_2)
			{
				aux_2->next = aux_1->next;
				aux_2->next->previus = aux_2;
			}
			free(aux_1->str);
			aux_1->str = NULL;
			free(aux_1);
			aux_1 = NULL;
		}
		if (aux_1)
			aux_1 = aux_1->next;
		else
			aux_1 = aux_2;
	}
}

t_token	*tokenization_cmd(t_token *tokens)
{
	t_token	*aux;
	int		cicle;

	cicle = 0;
	tokens = build_tokens();
	if (!tokens)
		return (NULL);
	delete_null_tokens(&tokens);
	if (!tokens)
		return (NULL);
	aux = tokens;
	while (1)
	{
		multiplex_fuction(tokens, cicle);
		if (tokens->next == NULL && cicle == 0)
		{
			tokens = aux;
			cicle = 1;
			continue ;
		}
		else if (tokens->next == NULL && cicle == 1)
			break ;
		tokens = tokens->next;
	}
	return (aux);
}
