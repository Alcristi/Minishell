/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:25:47 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/04 18:49:23 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*new_token(char *data)
{
	t_token	*list;

	list = ft_calloc(sizeof(t_token), 1);
	list->next = NULL;
	list->previus = NULL;
	list->str = ft_strdup(data);
	list->is_arg = FALSE;
	list->is_cmd = FALSE;
	list->is_heredoc = FALSE;
	list->is_input = FALSE;
	list->is_output = FALSE;
	list->is_out_append = FALSE;
	list->is_pipe = FALSE;
	list->quotes = FALSE;
	return (list);
}

void	add_node_last_token(t_token **node, char *data)
{
	t_token	*new_node;
	t_token	*aux;

	aux = *node;
	while (node[0]->next)
		*node = node[0]->next;
	new_node = new_token(data);
	new_node->previus = *node;
	node[0]->next = new_node;
	*node = aux;
}

void	add_node_middle_token(t_token **node, char *data)
{
	t_token	*new_node;
	t_token	*aux;

	aux = node[0]->next;
	if (!aux)
	{
		new_node = new_token(data);
		node[0]->next = new_node;
		new_node->next = aux;
		new_node->previus = node[0];
	}
	else
	{
		new_node = new_token(data);
		node[0]->next = new_node;
		new_node->next = aux;
		aux->previus = new_node;
		new_node->previus = node[0];
	}
}

void	free_token(t_token **head)
{
	t_token	*aux;

	while (head[0])
	{
		aux = head[0]->next;
		if (head[0]->str)
			free(head[0]->str);
		head[0]->str = NULL;
		free(head[0]);
		head[0] = aux;
	}
	head[0] = NULL;
}

int	has_quotes(char *str)
{
	int	count;

	count = -1;
	if (str[0] == '>' || str[0] == '<' || str[0] == '|')
		return (FALSE);
	while (str[++count])
		if (str[count] == '\'' || str[count] == '\"')
			return (TRUE);
	return (FALSE);
}
