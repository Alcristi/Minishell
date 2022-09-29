/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:57:02 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/29 19:02:59 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	validator_redirect_pipe(t_stacks *stacks, t_token *tokens
	, int *pid_child, int count)
{
	int	code_exit;

	if ((g_core_var->exit_code == INTERRUPT_SIG_INT
			|| g_core_var->exit_code == EXIT_FAILURE) && pid_child[count] == 0)
	{
		code_exit = g_core_var->exit_code;
		free(pid_child);
		close(g_core_var->fd_stdin);
		close(g_core_var->fd_stdout);
		free_exec(&stacks, &tokens);
		exit(code_exit);
	}
}

static void	exit_exec(int *pid_child)
{
	dup2(g_core_var->fd_stdin, STDIN_FILENO);
	if (g_core_var->fd_out != 0)
		dup2(g_core_var->fd_stdout, STDOUT_FILENO);
	handle_wait(pid_child);
	close(g_core_var->fd_stdin);
	close(g_core_var->fd_stdout);
	free(pid_child);
}

static void	next_cmd(t_token **cursor, t_stacks *stacks)
{
	if (stacks->stack_herodoc)
		signal(SIGQUIT, SIG_IGN);
	while (cursor[0] && !cursor[0]->is_pipe)
		cursor[0] = cursor[0]->next;
	if (cursor[0])
		cursor[0] = cursor[0]->next;
}

static int	*init_exec_pipe(t_token **cursor, t_stacks *stacks,
				int quantity_cmd, int *count)
{
	int	*pid;

	g_core_var->fd_stdout = dup(STDOUT_FILENO);
	g_core_var->fd_stdin = dup(STDIN_FILENO);
	cursor[0] = stacks->stack_cmd;
	*count = 0;
	pid = ft_calloc(sizeof(int), quantity_cmd * 2);
	return (pid);
}

void	exec_with_pipe(t_stacks **stacks, t_token **tokens, int quantity_cmd)
{
	int		count;
	int		*pid_child;
	t_token	*cursor;
	int		fd_pipe[2];

	pid_child = init_exec_pipe(&cursor, stacks[0], quantity_cmd, &count);
	exec_here_doc(stacks[0], tokens[0], pid_child, count);
	while (cursor)
	{
		handle_pipe(count, quantity_cmd, fd_pipe);
		pid_child[count] = fork();
		validate_fork(pid_child[count]);
		handle_redirect_pipe(stacks[0], tokens[0], pid_child, count);
		validator_redirect_pipe(stacks[0], tokens[0], pid_child, count);
		signal(SIGQUIT, handle_quit);
		if (pid_child[count] == 0)
		{
			close(fd_pipe[0]);
			exec_in_pipe(stacks, tokens, pid_child, count);
		}
		else
			next_cmd(&cursor, stacks[0]);
		count++;
	}
	exit_exec(pid_child);
}
