/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:25:47 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/26 14:19:01 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//cria um novo token utilizando como conteudo o char *data
t_token	*new_token(char *data)
{
	t_token	*list;

	list = malloc(sizeof(t_token));
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
	list->single_quotes = FALSE;
	list->double_quotes = FALSE;
	return (list);
}

//adiciona um novo token ao fim da lista linkada
//ultiliza char *data como conteudo do novo token
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

//SUPOSIÇÃO: adiciona um novo token na posição atual da lista linkada
//ultiliza char *data como conteudo do novo token
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

//Faz a limpeza dos tokens
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
