/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stackTokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 10:52:42 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/04 20:08:16 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	load_stacks(t_token **dst, t_token *data)
{
	t_token	*new_dst;
	char	*str;

	str = ft_strdup(data->str);
	if (!*dst)
	{
		dst[0] = ft_calloc(sizeof(t_token), 1);
		dst[0][0] = *data;
		dst[0][0].str = str;
		dst[0][0].next = NULL;
		dst[0][0].previus = NULL;
	}
	else
	{
		new_dst = ft_calloc(sizeof(t_token), 1);
		*new_dst = *data;
		new_dst->str = str;
		new_dst->previus = NULL;
		new_dst->next = dst[0];
		dst[0]->previus = new_dst;
		dst[0] = dst[0]->previus;
	}
}

static void	load_stacks_last(t_token **dst, t_token *data)
{
	t_token	*new_dst;
	t_token	*cursor;
	char	*str;

	str = ft_strdup(data->str);
	if (!*dst)
	{
		dst[0] = ft_calloc(sizeof(t_token), 1);
		dst[0][0] = *data;
		dst[0][0].str = str;
		dst[0][0].next = NULL;
		dst[0][0].previus = NULL;
	}
	else
	{
		new_dst = ft_calloc(sizeof(t_token), 1);
		*new_dst = *data;
		cursor = dst[0];
		while (cursor->next)
			cursor = cursor->next;
		new_dst->str = str;
		new_dst->previus = cursor;
		new_dst->next = NULL;
		cursor->next = new_dst;
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
			load_stacks_last(&new->stack_input, cursor);
		else if ((cursor->is_output || cursor->is_out_append)
			&& cursor->str[0] != '>')
			load_stacks_last(&new->stack_out, cursor);
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
	*stacks = NULL;
}
