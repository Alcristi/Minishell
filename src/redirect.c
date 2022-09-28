/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:50:57 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/28 11:55:39 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	select_stdin(t_token *tokens)
{
	t_token	*cursor;
	int		priority_stdin;

	priority_stdin = -1;
	cursor = tokens;
	while (cursor)
	{
		if (cursor->is_heredoc)
			priority_stdin = 2;
		else if (cursor->is_input)
			priority_stdin = 1;
		cursor = cursor->next;
	}
	return (priority_stdin);
}

void	file_error(char *str)
{
	write(2, "Minishell : ", 13);
	ft_putstr_fd(str, 2);
	write(2, ": ", 2);
	perror(NULL);
	g_core_var->exit_code = EXIT_FAILURE;
}

void	open_out(t_stacks *stacks)
{
	if (stacks->stack_out->is_output)
		g_core_var->fd_out = open(stacks->stack_out->str,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		g_core_var->fd_out = open(stacks->stack_out->str,
				O_WRONLY | O_APPEND | O_CREAT, 0644);
}

void	handle_redirect(t_stacks *stacks, t_token *tokens, int *pid, int count)
{
	int	select;

	select = select_stdin(tokens);
	if (stacks->stack_input && select == 1 && pid[count] == 0)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
			file_error(stacks->stack_input->str);
		dup2(g_core_var->fd_in, STDIN_FILENO);
	}
	if (stacks->stack_out && pid[count] == 0)
	{
		open_out(stacks);
		if (g_core_var->fd_out < 0)
			file_error(stacks->stack_out->str);
	}
}

void	handle_redirect_pipe(t_stacks *stacks, t_token *tokens
	, int *pid, int count)
{
	int	select;

	select = select_stdin(tokens);
	if (stacks->stack_input && select == 1 && pid[count] == 0 && count == 0)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
			file_error(stacks->stack_input->str);
		dup2(g_core_var->fd_in, STDIN_FILENO);
	}
	if (stacks->stack_out && pid[count] == 0 && count == (amount_pipe(stacks)))
	{
		open_out(stacks);
		if (g_core_var->fd_out < 0)
			file_error(stacks->stack_out->str);
		copy_fd(g_core_var->fd_out, STDOUT_FILENO);
	}
}
