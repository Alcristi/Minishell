/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:51:01 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/25 02:25:38 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
}

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
	if (is_valid(temp_stack->stack_cmd))
	{
		arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
		arg_cmd_build(arg_cmd, count_tokens, temp_stack);
		return (arg_cmd);
	}
	else
		return (NULL);
}

//abre os arquivos de entrada e de saida
void	open_file(t_stacks *stacks)
{
	if (stacks->stack_input)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
			exit(1);
		dup2(g_core_var->fd_in, 0);
	}
	if (stacks->stack_out)
	{
		if (stacks->stack_out->is_output)
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (g_core_var->fd_out < 0)
			exit(1);
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
			if (!ft_strncmp(line, stacks->stack_herodoc->str,
					ft_strlen(stacks->stack_herodoc->str)))
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
