/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:53:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/29 14:15:05 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**load_path(void)
{
	t_double_list	*aux_env;
	char			*path;
	char			**cut_path;

	aux_env = g_core_var->env;
	while (aux_env)
	{
		if (search_var("PATH", aux_env->data))
		{
			path = cat_var(aux_env->data, 4);
			break ;
		}
		else
			aux_env = aux_env->next;
	}
	cut_path = ft_split(path, ':');
	free(path);
	return (cut_path);
}

void	free_exec(t_stacks **stacks, t_token **tokens)
{
	free_stacks(stacks);
	free_token(tokens);
	free_core();
	rl_clear_history();
	stacks = NULL;
	tokens = NULL;
}

void	validate_redirect(t_stacks *stacks, t_token *tokens)
{
	int	code_exit;

	if (g_core_var->exit_code == INTERRUPT_SIG_INT
		|| g_core_var->exit_code == EXIT_FAILURE)
	{
		code_exit = g_core_var->exit_code;
		free_exec(&stacks, &tokens);
		exit(code_exit);
	}
}

void	exec_here_cmd(t_stacks *stacks, t_token *tokens, int pid)
{
	int	select;

	select = select_stdin(tokens);
	if (stacks->stack_herodoc && pid == 0)
	{
		g_core_var->exit_code = here_doc(stacks, tokens, select);
		if (g_core_var->exit_code != 0)
			g_core_var->exit_code = INTERRUPT_SIG_INT;
	}
}

void	exec_cmd(t_stacks *stacks, t_token *tokens)
{
	char	**cmd;
	pid_t	pid;
	int		status;
	char	**envp;

	pid = fork();
	exec_here_cmd(stacks, tokens, pid);
	validate_fork(pid);
	signal(SIGQUIT, handle_quit);
	if (pid == 0)
	{
		handle_redirect(stacks, tokens, &pid, 0);
		validate_redirect(stacks, tokens);
		cmd = build_cmd(&stacks, &tokens, 0);
		if (g_core_var->fd_out != 0)
			dup2(g_core_var->fd_out, STDOUT_FILENO);
		if (cmd)
		{
			envp = convert_env_for_string();
			execve(cmd[0], cmd, envp);
		}
		exec_fail(&stacks, &tokens, cmd, envp);
	}
	else
		parent(pid, stacks);
}

void	redirect_without_cmd(t_stacks *stacks, t_token *tokens)
{
	int	select;

	select = 3;
	if (stacks->stack_herodoc)
	{
		g_core_var->exit_code = here_doc(stacks, tokens, select);
		if (g_core_var->exit_code != 0)
			g_core_var->exit_code = INTERRUPT_WITHOUT_CMD ;
	}
	if (stacks->stack_input)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
			file_error(stacks->stack_input->str);
		else
			close(g_core_var->fd_in);
	}
	if (stacks->stack_out)
	{
		open_out(stacks);
		if (g_core_var->fd_out < 0)
			file_error(stacks->stack_out->str);
		else
			close(g_core_var->fd_out);
	}
}

void	execute(t_stacks **stacks, t_token **tokens)
{
	int	quantity_pipe;

	quantity_pipe = amount_pipe(stacks[0]);
	signal(SIGINT, handle);
	if (stacks[0]->stack_cmd)
	{
		if (quantity_pipe)
			exec_with_pipe(stacks, tokens, quantity_pipe + 1);
		else
		{
			if (is_builtin(stacks[0]))
				exec_builtin(stacks[0], tokens[0]);
			else
				exec_cmd(stacks[0], tokens[0]);
		}
	}
	else
		redirect_without_cmd(stacks[0], tokens[0]);
}
