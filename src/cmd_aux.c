/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_aux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 00:05:17 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/20 18:39:43 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	number_tokens(t_stacks *stack)
// {
// 	t_token		*cursor;
// 	int			count_tokens;

// 	count_tokens = 0;
// 	cursor = stack->stack_cmd;
// 	while (cursor && !cursor->is_pipe)
// 	{
// 		count_tokens++;
// 		cursor = cursor->next;
// 	}
// 	return (count_tokens);
// }

// static t_stacks	*position_cmd(t_stacks *stack, int id)
// {
// 	int			position;
// 	t_stacks	*temp_stack;

// 	position = 0;
// 	temp_stack = stack;
// 	while (1)
// 	{
// 		if (temp_stack->stack_cmd->is_cmd)
// 		{
// 			position++;
// 			if (position > id)
// 				break ;
// 		}
// 		temp_stack->stack_cmd = temp_stack->stack_cmd->next;
// 	}
// 	return (temp_stack);
// }

// static void	free_cmd_aux(t_token **previus)
// {
// 	free(previus[0]);
// 	previus[0] = NULL;
// }

// static void	arg_cmd_build(char **arg_cmd, int ct_tokens, t_stacks *stack)
// {
// 	int		count;

// 	count = 0;
// 	while (count < ct_tokens && stack->stack_cmd)
// 	{
// 		arg_cmd[count] = ft_strdup(stack->stack_cmd->str);
// 		count++;
// 		if (!stack->stack_cmd->next)
// 		{
// 			free_cmd_aux(&stack->stack_cmd);
// 			break ;
// 		}
// 		stack->stack_cmd = stack->stack_cmd->next;
// 		if (stack->stack_cmd)
// 			free_cmd_aux(&stack->stack_cmd->previus);
// 	}
// 	if (stack->stack_cmd && stack->stack_cmd->is_pipe)
// 	{
// 		stack->stack_cmd = stack->stack_cmd->next;
// 		free_cmd_aux(&stack->stack_cmd->previus);
// 	}
// 	arg_cmd[count] = NULL;
// }

// char	**build_cmd(t_stacks *stack, int id)
// {
// 	t_stacks	*temp_stack;
// 	char		**arg_cmd;
// 	int			count_tokens;
// 	int			count;

// 	count = 0;
// 	temp_stack = position_cmd(stack, id);
// 	count_tokens = number_tokens(temp_stack);
// 	if (is_valid(temp_stack->stack_cmd))
// 	{
// 		arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
// 		arg_cmd_build(arg_cmd, count_tokens, temp_stack);
// 		return (arg_cmd);
// 	}
// 	else
// 		return (NULL);
// }

//funcao auxiliar da cmd_build
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
	}
	return (temp_stack);
}

//faz a contagem de tokens
//funcao auxiliar da cmd_build
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

//preciso diminuir linhas
//funcao auxiliar da build_cmd que constroi o arg_cmd
static void	arg_cmd_build(char **arg_cmd, int ct_tokens, t_stacks *stack)
{
	int		count;

	count = 0;
	/***
	* Porque a limpeza esta sendo feita dentro do algoritmo
	* ao invez de usar uma função que limpe tudo no fim do ciclo?
	***/
	while (count < ct_tokens && stack->stack_cmd)
	{
		arg_cmd[count] = ft_strdup(stack->stack_cmd->str);
		count++;
		if (!stack->stack_cmd->next)
		{
			free(stack->stack_cmd->str);
			free(stack->stack_cmd);
			stack->stack_cmd = NULL;
			break ;
		}
		stack->stack_cmd = stack->stack_cmd->next;
		if (stack->stack_cmd)
		{
			free(stack->stack_cmd->previus->str);
			free(stack->stack_cmd->previus);
			stack->stack_cmd->previus = NULL;
		}
	}
	if (stack->stack_cmd && stack->stack_cmd->is_pipe)
	{
		stack->stack_cmd = stack->stack_cmd->next;
		free(stack->stack_cmd->previus->str);
		free(stack->stack_cmd->previus);
		stack->stack_cmd->previus = NULL;
	}
	arg_cmd[count] = NULL;
}

// int is_builtin(t_stacks *cmd)
// {
// 	if (!ft_strncmp(cmd->stack_cmd->str,"echo",ft_strlen("echo")))
// 		return (1);
// 	else if (!ft_strncmp(cmd->stack_cmd->str,"cd",ft_strlen("cd")))
// 		return (1);
// 	else if (!ft_strncmp(cmd->stack_cmd->str,"pwd",ft_strlen("pwd")))
// 		return (1);
// 	else if (!ft_strncmp(cmd->stack_cmd->str,"export",ft_strlen("export")))
// 		return (1);
// 	else if (!ft_strncmp(cmd->stack_cmd->str,"unset",ft_strlen("unset")))
// 		return (1);
// 	else if (!ft_strncmp(cmd->stack_cmd->str,"env",ft_strlen("env")))
// 		return (1);
// 	else
// 		return (0);
// }

//constroi a matriz de execução dos comandos no execve
char	**build_cmd(t_stacks *stack, int id)
{
	t_stacks	*temp_stack;
	char		**arg_cmd;
	int			count_tokens;
	int			count;

	count = 0;
	temp_stack = position_cmd(stack, id);
	count_tokens = number_tokens(temp_stack);
	if (is_builtin(temp_stack))
	{
		arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
		arg_cmd_build(arg_cmd, count_tokens, temp_stack);
		return (arg_cmd);
	}
	if (is_valid(temp_stack->stack_cmd))
	{
		arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
		arg_cmd_build(arg_cmd, count_tokens, temp_stack);
		return (arg_cmd);
	}
	else
		return (NULL);
}
