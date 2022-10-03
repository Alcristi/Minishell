/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:57:04 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/03 21:33:14 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	close_file_child(int *pid_child)
{
	close(g_core_var->fd_stdin);
	close(g_core_var->fd_stdout);
	free(pid_child);
}

void	exec_fail(t_stacks **stacks, t_token **tokens, char **cmd, char **envp)
{
	free_double(envp);
	free_double(cmd);
	free_exec(stacks, tokens);
	perror(NULL);
	exit(CMD_NOT_FOUND);
}

void	exec_in_pipe(t_stacks **stacks, t_token **tokens
	, int *pid_child, int count)
{
	char	**cmd;
	int		exit_code;
	char	**envp;

	init_point(&cmd, &envp);
	close_file_child(pid_child);
	cmd = build_cmd(stacks, tokens, count);
	if (cmd)
	{
		if (is_builtin_cmd(cmd[0]))
		{
			if (!ft_strncmp(cmd[0], "exit", ft_strlen("exit")))
				free_exit(stacks, tokens);
			exit_code = execute_builtin(cmd);
			exit_builtin_in_pipe(cmd, stacks, tokens, exit_code);
		}
		else
		{
			envp = convert_env_for_string();
			execve(cmd[0], cmd, envp);
		}
		exec_fail(stacks, tokens, cmd, envp);
	}
}

void	handle_pipe(int count, int quantity_cmd, int *fd_pipe)
{
	if (count > 0)
		copy_fd(fd_pipe[0], STDIN_FILENO);
	if (count >= 0 && count != (quantity_cmd - 1))
	{
		if (pipe(fd_pipe) == -1)
			exit(EXIT_FAILURE);
		copy_fd(fd_pipe[1], STDOUT_FILENO);
	}
	else if (count == quantity_cmd - 1)
	{
		close(fd_pipe[0]);
		dup2(g_core_var->fd_stdout, STDOUT_FILENO);
	}
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
