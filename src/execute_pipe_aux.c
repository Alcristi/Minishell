/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:57:04 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/28 22:26:25 by alcristi         ###   ########.fr       */
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

int	is_builtin_cmd(char *cmd)
{
	if (!ft_strncmp(cmd, "echo", ft_strlen("echo")))
		return (1);
	else if (!ft_strncmp(cmd, "cd", ft_strlen("cd")))
		return (1);
	else if (!ft_strncmp(cmd, "pwd", ft_strlen("pwd")))
		return (1);
	else if (!ft_strncmp(cmd, "export", ft_strlen("export")))
		return (1);
	else if (!ft_strncmp(cmd, "unset", ft_strlen("unset")))
		return (1);
	else if (!ft_strncmp(cmd, "env", ft_strlen("env")))
		return (1);
	else if (!ft_strncmp(cmd, "exit", ft_strlen("exit")))
		return (1);
	else
		return (0);
}

void	free_exit(t_stacks **stacks, t_token **tokens)
{
	free_stacks(stacks);
	free_token(tokens);
}

void	exec_in_pipe(t_stacks **stacks, t_token **tokens
	, int *pid_child, int count)
{
	char	**cmd;
	int		exit_code;

	close_file_child(pid_child);
	cmd = build_cmd(stacks, tokens, count);
	if (cmd)
	{
		if (is_builtin_cmd(cmd[0]))
		{
			if (!ft_strncmp(cmd[0], "exit", ft_strlen("exit")))
				free_exit(stacks, tokens);
			exit_code = execute_builtin(cmd);
			free_double(cmd);
			free_exec(stacks, tokens);
			exit (exit_code);
		}
		else
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
