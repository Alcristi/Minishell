/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_method.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:04:06 by alcristi          #+#    #+#             */
/*   Updated: 2022/06/15 15:27:12 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_double_list	*new(char *data)
{
	t_double_list	*list;

	list = malloc(sizeof(t_double_list));
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
	while (node)
	{
		printf("%s\n", node->data);
		node = node->next;
	}
}

void	free_list(t_double_list *head)
{
	t_double_list	*aux;

	while (head)
	{
		aux = head->next;
		free(head->data);
		free(head);
		head = aux;
	}
}
