/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:57:04 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/23 13:20:34 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	close_file_child(int *pid_child)
{
	close(g_core_var->fd_stdin);
	close(g_core_var->fd_stdout);
	close(g_core_var->fd_pipe[0]);
	close(g_core_var->fd_pipe[1]);
	free(pid_child);
}

void	exec_in_pipe(t_stacks *stacks, t_token *tokens
	, int *pid_child, int count)
{
	char	**cmd;

	close_file_child(pid_child);
	cmd = build_cmd(stacks, tokens, count);
	if (cmd)
	{
		execve(cmd[0], cmd, g_core_var->envp);
		free_double(cmd);
	}
	free_exec(stacks, tokens);
	perror(NULL);
	exit(CMD_NOT_FOUND);
}

void	handle_pipe(int count, int quantity_cmd, int out_origin)
{
	int	std_out;
	int	origin_stdout;

	origin_stdout = dup(out_origin);
	if (count > 0)
		copy_fd(g_core_var->fd_pipe[0], STDIN_FILENO);
	if (count >= 0 && count != (quantity_cmd - 1))
	{
		if (pipe(g_core_var->fd_pipe) == -1)
			exit(EXIT_FAILURE);
		copy_fd(g_core_var->fd_pipe[1], STDOUT_FILENO);
	}
	else if (count == quantity_cmd - 1)
	{
		close(g_core_var->fd_pipe[0]);
		dup2(origin_stdout, STDOUT_FILENO);
	}
	close(origin_stdout);
}

void	exec_here_doc(t_stacks *stacks, t_token *tokens, int *pid, int count)
{
	int	select;

	select = select_stdin(tokens);
	if (stacks->stack_herodoc && count == 0)
	{
		g_core_var->exit_code = here_doc_pipe(stacks, tokens, select, pid);
		if (g_core_var->exit_code != 0)
			g_core_var->exit_code = INTERRUPT_SIG_INT;
	}
}
