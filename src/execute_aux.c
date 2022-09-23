/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:31:22 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/22 23:48:14 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	is_builtin(t_stacks *cmd)
{
	if (!ft_strncmp(cmd->stack_cmd->str, "echo", ft_strlen("echo")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str, "cd", ft_strlen("cd")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str, "pwd", ft_strlen("pwd")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str, "export", ft_strlen("export")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str, "unset", ft_strlen("unset")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str, "env", ft_strlen("env")))
		return (1);
	else
		return (0);
}

void	handle_wait(int *pid)
{
	int	count;

	count = 0;
	while (pid[count] != 0)
	{
		waitpid(pid[count], &g_core_var->exit_code, 0);
		count++;
	}
}

void	copy_fd(int font, int dest)
{
	dup2(font, dest);
	close(font);
}

void	parent(int pid)
{
	waitpid (pid, &g_core_var->exit_code, 0);
	if (g_core_var->fd_in != 0)
		close(g_core_var->fd_in);
	if (g_core_var->fd_out != 0)
		close(g_core_var->fd_out);
}
