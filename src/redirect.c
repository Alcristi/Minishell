/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:50:57 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/04 20:34:57 by alcristi         ###   ########.fr       */
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
	if (!access(str, F_OK))
	{
		if (access(str, X_OK))
		{
			error_message(str, ": Permission denied\n");
			g_core_var->exit_code = EXIT_FAILURE;
		}
	}
	else
	{
		error_message(str, ": No such file or directory\n");
		g_core_var->exit_code = EXIT_FAILURE;
	}
}

char	*open_out(t_stacks *stacks)
{
	t_token *cursor;

	cursor = stacks->stack_out;
	while (cursor)
	{
		if (cursor->is_output)
			g_core_var->fd_out = open(cursor->str,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			g_core_var->fd_out = open(cursor->str,
				O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (g_core_var->fd_out == -1)
			return (cursor->str);
		cursor = cursor->next;
		if (cursor)
			close (g_core_var->fd_out);
	}
	return (NULL);
}

char	*open_in(t_stacks *stacks)
{
	t_token *cursor;

	cursor = stacks->stack_input;
	while (cursor)
	{
		g_core_var->fd_in = open(cursor->str, O_RDONLY);
		if (g_core_var->fd_in == -1)
			return (cursor->str);
		cursor = cursor->next;
		if (cursor)
			close (g_core_var->fd_in);
	}
	return (NULL);
}

void	handle_redirect(t_stacks *stacks, t_token *tokens, int *pid, int count)
{
	int		select;
	char	*out;

	select = select_stdin(tokens);
	if (stacks->stack_input && select == 1 && pid[count] == 0)
	{
		out = open_in(stacks);
		if (g_core_var->fd_in < 0)
			file_error(out);
		else
			dup2(g_core_var->fd_in, STDIN_FILENO);
	}
	if (stacks->stack_out && pid[count] == 0 && g_core_var->fd_in >= 0)
	{
		out = 	open_out(stacks);
		if (g_core_var->fd_out < 0)
			file_error(out);
	}
}

void	handle_redirect_pipe(t_stacks *stacks, t_token *tokens
	, int *pid, int count)
{
	int	select;
	char *out;

	select = select_stdin(tokens);
	if (stacks->stack_input && select == 1 && pid[count] == 0 && count == 0)
	{
		out = open_in(stacks);
		if (g_core_var->fd_in < 0)
			file_error(out);
		else
			dup2(g_core_var->fd_in, STDIN_FILENO);
	}
	if (stacks->stack_out && pid[count] == 0 && count == (amount_pipe(stacks)) && g_core_var->fd_in >= 0)
	{
		out = open_out(stacks);
		if (g_core_var->fd_out < 0)
			file_error(out);
		else
			copy_fd(g_core_var->fd_out, STDOUT_FILENO);
	}
}
