/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 00:05:17 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/30 11:19:01 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_stacks	*position_cmd(t_stacks *stack, int id)
{
	int			position;
	t_stacks	*temp_stack;

	position = 0;
	temp_stack = stack;
	while (1)
	{
		if (temp_stack->stack_cmd->is_cmd)
		{
			position++;
			if (position > id)
				break ;
		}
		temp_stack->stack_cmd = temp_stack->stack_cmd->next;
		if (temp_stack->stack_cmd->previus)
			free_two(&temp_stack->stack_cmd->previus->str,
				&temp_stack->stack_cmd->previus);
	}
	return (temp_stack);
}

static int	number_tokens(t_stacks *stack)
{
	t_token		*cursor;
	int			count_tokens;

	count_tokens = 0;
	cursor = stack->stack_cmd;
	while (cursor && !cursor->is_pipe)
	{
		count_tokens++;
		cursor = cursor->next;
	}
	return (count_tokens);
}

void	free_two(char **str, t_token **cmd)
{
	free(str[0]);
	str[0] = NULL;
	free(cmd[0]);
	cmd[0] = NULL;
}

void	arg_cmd_build(char **arg_cmd, int ct_tokens, t_stacks *stack)
{
	int		count;

	count = 0;
	while (count < ct_tokens && stack->stack_cmd)
	{
		arg_cmd[count] = ft_strdup(stack->stack_cmd->str);
		count++;
		if (!stack->stack_cmd->next)
		{
			free_two(&stack->stack_cmd->str, &stack->stack_cmd);
			break ;
		}
		stack->stack_cmd = stack->stack_cmd->next;
		if (stack->stack_cmd)
			free_two(&stack->stack_cmd->previus->str,
				&stack->stack_cmd->previus);
	}
	if (stack->stack_cmd && stack->stack_cmd->is_pipe)
	{
		stack->stack_cmd = stack->stack_cmd->next;
		free_two(&stack->stack_cmd->previus->str,
			&stack->stack_cmd->previus);
	}
	arg_cmd[count] = NULL;
}

char	**build_cmd(t_stacks **stack, t_token **tokens, int id)
{
	t_stacks	*temp_stack;
	char		**arg_cmd;
	int			count_tokens;

	temp_stack = position_cmd(stack[0], id);
	count_tokens = number_tokens(temp_stack);
	if (is_builtin(temp_stack))
	{
		arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
		arg_cmd_build(arg_cmd, count_tokens, temp_stack);
		return (arg_cmd);
	}
	if (is_valid(temp_stack->stack_cmd, stack, tokens))
	{
		arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
		arg_cmd_build(arg_cmd, count_tokens, temp_stack);
		return (arg_cmd);
	}
	else
		return (NULL);
}
