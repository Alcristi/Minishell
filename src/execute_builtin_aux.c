/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_aux.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:21:40 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/04 19:32:55 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	int		select;
	char	*out;
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
		out = open_out(stacks);
		if (g_core_var->fd_out < 0)
			file_error(out);
	}
}

void	exit_builtin_in_pipe(char **cmd, t_stacks **stacks,
			t_token **tokens, int exit_code)
{
	free_double(cmd);
	free_exec(stacks, tokens);
	exit (exit_code);
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
