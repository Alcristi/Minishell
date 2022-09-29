/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 00:01:05 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/29 19:25:39 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	exec_echo(char **cmd)
{
	int	count_args;

	count_args = 0;
	while (cmd[count_args])
		count_args++;
	if (count_args - 1)
		return (bt_echo(count_args - 1, &cmd[1]));
	return (bt_echo(count_args - 1, NULL));
}

int	execute_builtin(char **cmd)
{
	if (!ft_strncmp(cmd[0], "echo", ft_strlen("echo")))
		return (exec_echo(cmd));
	else if (!ft_strncmp(cmd[0], "cd", ft_strlen("cd")))
		return (bt_cd(cmd[1]));
	else if (!ft_strncmp(cmd[0], "pwd", ft_strlen("pwd")))
		return (bt_pwd());
	else if (!ft_strncmp(cmd[0], "export", ft_strlen("export")))
		return (bt_export(cmd[1]));
	else if (!ft_strncmp(cmd[0], "unset", ft_strlen("unset")))
		return (bt_unset(cmd[1]));
	else if (!ft_strncmp(cmd[0], "env", ft_strlen("env")))
	{
		bt_env();
		return (0);
	}
	else if (!ft_strncmp(cmd[0], "exit", ft_strlen("exit")))
		bt_exit(cmd);
	return (0);
}

static int	validate_redirect_builtin(void)
{
	if (g_core_var->exit_code == INTERRUPT_SIG_INT
		|| g_core_var->exit_code == EXIT_FAILURE)
		return (1);
	return (0);
}

static void	close_file_builtin(void)
{
	close(g_core_var->fd_stdin);
	if (g_core_var->fd_out != 0)
	{
		close(g_core_var->fd_out);
		dup2(g_core_var->fd_stdout, STDOUT_FILENO);
		close(g_core_var->fd_stdout);
	}
	else
		close(g_core_var->fd_stdout);
}

void	exec_here_builtin(t_stacks *stacks, t_token *tokens, int pid)
{
	int	select;

	select = 0;
	if (stacks->stack_herodoc && pid == 0)
	{
		g_core_var->exit_code = here_doc(stacks, tokens, select);
		if (g_core_var->exit_code != 0)
			g_core_var->exit_code = INTERRUPT_SIG_INT;
	}
}

void	handle_redirect_builtin(t_stacks *stacks, t_token *tokens
		, int *pid, int count)
{
	int	select;

	select = select_stdin(tokens);
	if (stacks->stack_input && select == 1 && pid[count] == 0)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
			file_error(stacks->stack_input->str);
		else
			close(g_core_var->fd_in);
	}
	if (stacks->stack_out && pid[count] == 0)
	{
		open_out(stacks);
		if (g_core_var->fd_out < 0)
			file_error(stacks->stack_out->str);
	}
}

void	exec_builtin(t_stacks *stacks, t_token *tokens)
{
	char	**cmd;
	int		pid;

	pid = 0;
	g_core_var->fd_stdout = dup(STDOUT_FILENO);
	g_core_var->fd_stdin = dup(STDIN_FILENO);
	exec_here_builtin(stacks, tokens, pid);
	handle_redirect_builtin(stacks, tokens, &pid, 0);
	if (validate_redirect_builtin ())
		return ;
	cmd = build_cmd(&stacks, &tokens, 0);
	if (g_core_var->fd_out != 0)
		dup2(g_core_var->fd_out, STDOUT_FILENO);
	else if (!ft_strncmp(cmd[0], "exit", ft_strlen("exit")))
	{
		free_token(&tokens);
		free_stacks(&stacks);
	}
	g_core_var->exit_code = execute_builtin(cmd);
	free_double(cmd);
	close_file_builtin();
}
