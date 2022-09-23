/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:31:31 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/22 23:31:32 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	exit_child(t_stacks *stacks, t_token *tokens, char *line)
{
	free(line);
	free_stacks(&stacks);
	free_token(&tokens);
	free_core();
	exit(0);
}

static void	heredoc_child(int fd_pp[2], t_stacks *stacks
	, t_token *tokens, int is_priority)
{
	char	*line;

	while (1)
	{
		write(STDIN_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, stacks->stack_herodoc->str,
				ft_strlen(stacks->stack_herodoc->str)))
		{
			close(fd_pp[1]);
			exit_child(stacks, tokens, line);
		}
		if (is_priority == 2)
			write(fd_pp[1], line, ft_strlen(line));
		free(line);
	}
}

static void	close_files_here_doc(int fd, int *pid)
{
	free(pid);
	close(g_core_var->fd_stdin);
	close(g_core_var->fd_stdout);
	close(g_core_var->fd_pipe[0]);
	close(fd);
}

int	here_doc(t_stacks *stacks, t_token *tokens, int is_priority)
{
	int		fd_pp[2];
	pid_t	pid_hd;
	char	*line;
	int		status;

	if (pipe(fd_pp) == -1)
		exit(EXIT_FAILURE);
	pid_hd = fork();
	if (pid_hd == 0)
	{
		close(fd_pp[0]);
		heredoc_child(fd_pp, stacks, tokens, is_priority);
	}
	else
	{
		close(fd_pp[1]);
		if (is_priority == 2)
			dup2(fd_pp[0], STDIN_FILENO);
		close(fd_pp[0]);
		waitpid(pid_hd, &status, 0);
		return (status);
	}
}

int	here_doc_pipe(t_stacks *stacks, t_token *tokens, int is_priority, int *pid)
{
	int		fd_pp[2];
	pid_t	pid_hd;
	char	*line;
	int		status;

	if (pipe(fd_pp) == -1)
		exit(EXIT_FAILURE);
	pid_hd = fork();
	if (pid_hd == 0)
	{
		close_files_here_doc(fd_pp[0], pid);
		heredoc_child(fd_pp, stacks, tokens, is_priority);
	}
	else
	{
		close(fd_pp[1]);
		if (is_priority == 2)
			dup2(fd_pp[0], STDIN_FILENO);
		close(fd_pp[0]);
		waitpid(pid_hd, &status, 0);
		return (status);
	}
}
