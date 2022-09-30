/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_method.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:04:06 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/30 02:59:18 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_double_list	*new(char *data)
{
	t_double_list	*list;

	list = ft_calloc(sizeof(t_double_list), 1);
	list->next = NULL;
	list->previus = NULL;
	list->data = ft_strdup(data);
	return (list);
}

void	add_node_front(t_double_list **node, char *data)
{
	t_double_list	*new_node;

	if (*node == NULL)
		*node = new(data);
	else
	{
		new_node = new(data);
		new_node->next = *node;
		node[0]->previus = new_node;
		new_node->previus = NULL;
		*node = new_node;
	}
}

void	add_node_last(t_double_list **node, char *data)
{
	t_double_list	*new_node;
	t_double_list	*aux;

	aux = *node;
	while (node[0]->next)
		*node = node[0]->next;
	new_node = new(data);
	new_node->previus = *node;
	node[0]->next = new_node;
	*node = aux;
}

void	print_linked_list(t_double_list *node)
{
	t_double_list *aux;

	aux = node;
	while (aux->previus != NULL)
		aux = aux->previus;
	while (aux != NULL)
	{
		printf("item:%s\n", aux->data);
		aux = aux->next;
	}
}

/*
void	print_linked_list(t_double_list *node)
{
	node = node->next;
	while (node)
	{
		printf("%s", node->data);
		node = node->next;
	}
	printf("\n");
}
*/

void	free_list(t_double_list *head)
{
	t_double_list	*aux;
	t_double_list	*temp;

	temp = head;
	while (temp->previus != NULL)
		temp = temp->previus;
	while (temp != NULL)
	{
		aux = temp->next;
		ft_strdel(&temp->data);
		free(temp);
		temp = aux;
	}
}
