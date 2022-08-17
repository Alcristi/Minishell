/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stackTokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 10:52:42 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/16 18:14:23 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	load_stacks(t_token **node, t_token *data)
{
	t_token	*new_node;
	t_token	aux;
	char	*str;

	str = ft_strdup(data->str);
	if (!*node)
	{
		node[0] = ft_calloc(sizeof(t_token), 1);
		node[0][0] = *data;
		node[0][0].str = str;
		node[0][0].next = NULL;
		node[0][0].previus = NULL;
	}
	else
	{
		new_node = ft_calloc(sizeof(t_token), 1);
		*new_node = *data;
		new_node->str = str;
		new_node->previus = NULL;
		new_node->next = node[0];
		node[0]->previus = new_node;
		node[0] = node[0]->previus;
	}
}

void	load_stacks_last(t_token **node, t_token *data)
{
	t_token	*new_node;
	t_token	*cursor;
	char	*str;

	str = ft_strdup(data->str);
	if (!*node)
	{
		node[0] = ft_calloc(sizeof(t_token), 1);
		node[0][0] = *data;
		node[0][0].str = str;
		node[0][0].next = NULL;
		node[0][0].previus = NULL;
	}
	else
	{
		new_node = ft_calloc(sizeof(t_token), 1);
		*new_node = *data;
		cursor = node[0];
		while(cursor->next)
			cursor = cursor->next;
		new_node->str = str;
		new_node->previus = cursor;
		new_node->next = NULL;
		cursor->next = new_node;
	}
}

t_stacks	*build_stack(t_token *tokens)
{
	t_stacks	*new;
	t_token		*cursor;

	cursor = tokens;
	new = ft_calloc(sizeof(t_stacks), 1);
	while (cursor)
	{
		if (cursor->is_input && cursor->str[0] != '<')
			load_stacks(&new->stack_input, cursor);
		else if ((cursor->is_output || cursor->is_out_append)
			&& cursor->str[0] != '>')
			load_stacks(&new->stack_out, cursor);
		else if (cursor->is_heredoc && cursor->str[0] != '<')
			load_stacks(&new->stack_herodoc, cursor);
		else if (cursor->is_cmd || cursor->is_arg || cursor->is_pipe)
			load_stacks_last(&new->stack_cmd, cursor);
		cursor = cursor->next;
	}
	return (new);
}

void	free_stacks(t_stacks **stacks)
{
	free_token(&stacks[0]->stack_cmd);
	free_token(&stacks[0]->stack_herodoc);
	free_token(&stacks[0]->stack_input);
	free_token(&stacks[0]->stack_out);
	free(stacks[0]);
	stacks = NULL;
}
