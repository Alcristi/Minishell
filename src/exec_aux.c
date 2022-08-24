/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:51:01 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/24 01:58:30 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


//constroi a matriz de execução dos comandos no execve
char	**build_cmd(t_stacks *stack, int id)
{
	t_token	*cursor;
	char	**arg_cmd;
	int		count_tokens;
	int		count;
	int		position_cmd;

	position_cmd = 0;
	count = 0;
	count_tokens = 0;
	while (1)
	{
		if (stack->stack_cmd->is_cmd)
		{
			position_cmd++;
			if (position_cmd > id)
				break ;
		}
		stack->stack_cmd = stack->stack_cmd->next;
	}
	cursor = stack->stack_cmd;
	while (cursor && !cursor->is_pipe)
	{
		count_tokens++;
		cursor = cursor->next;
	}
	arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
	if (is_valid(stack->stack_cmd))
	{
		while (count < count_tokens && stack->stack_cmd)
		{
			arg_cmd[count] = ft_strdup(stack->stack_cmd->str);
			count++;
			if (!stack->stack_cmd->next)
			{
				free(stack->stack_cmd);
				stack->stack_cmd = NULL;
				break ;
			}
			stack->stack_cmd = stack->stack_cmd->next;
			if (stack->stack_cmd)
			{
				free(stack->stack_cmd->previus);
				stack->stack_cmd->previus = NULL;
			}
		}
		if (stack->stack_cmd && stack->stack_cmd->is_pipe)
		{
			stack->stack_cmd = stack->stack_cmd->next;
			free(stack->stack_cmd->previus);
			stack->stack_cmd->previus = NULL;
		}
		arg_cmd[count] = NULL;
		return (arg_cmd);
	}
	else
	{
		free(arg_cmd);
		return (NULL);
	}
}

static void	exit_child(t_stacks *stacks, t_token *tokens, char *line)
{
	free(line);
	free_stacks(&stacks);
	free_token(&tokens);
	free_core();
	exit(0);
}

//abre um arquivo de entrada na entrada padrão
void	here_doc(t_stacks *stacks, t_token *tokens)
{
	int		fd_pp[2];
	pid_t	pid_hd;
	char	*line;

	if (pipe(fd_pp) == -1)
		exit(-1);
	pid_hd = fork();
	if (pid_hd == 0)
	{
		close(fd_pp[0]);
		while (1)
		{
			write(STDIN_FILENO, "> ", 2);
			line = get_next_line(STDIN_FILENO);
			if (!ft_strncmp(line, stacks->stack_herodoc->str, ft_strlen(stacks->stack_herodoc->str)))
				exit_child(stacks, tokens, line);
			write(fd_pp[1], line, ft_strlen(line));
			free(line);
		}
	}
	else
	{
		close(fd_pp[1]);
		dup2(fd_pp[0], STDIN_FILENO);
		wait(NULL);
	}
}

//verifica a quantidade de pipes que tem dentro do argumento passado no prompt
int	amount_pipe(t_stacks *stacks)
{
	t_token	*cursor;
	int		amount_pipe;

	amount_pipe = 0;
	cursor = stacks->stack_cmd;
	while (cursor)
	{
		if (cursor->is_pipe)
			amount_pipe++;
		cursor = cursor->next;
	}
	return (amount_pipe);
}