/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 23:31:31 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/29 23:45:06 by esilva-s         ###   ########.fr       */
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

char	*mount_message_eof(char *s)
{
	char	*str;

	str = ft_strdup("\nminishell: warning: heredoc ");
	str = ft_strjoin_gnl(str, "delimited by EOF - wanted '");
	str = ft_strjoin_gnl(str, s);
	return (ft_strjoin_gnl(str, "'\n"));
}

static void	heredoc_child(int fd_pp[2], t_stacks *stacks
	, t_token *tokens, int is_priority)
{
	char	*line;

	while (1)
	{
		signal(SIGINT, handle_here);
		line = readline("> ");
		if (line)
			line = ft_strjoin_gnl(line, "\n");
		if (line == NULL)
		{
			line = mount_message_eof(stacks->stack_herodoc->str);
			ft_putstr_fd(line, 2);
			close(fd_pp[1]);
			exit_child(stacks, tokens, line);
		}
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
	signal(SIGQUIT, SIG_IGN);
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
	return (-1);//adicionei para compilar
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
	signal(SIGQUIT, SIG_IGN);
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
	return (-1);//adicionei para compilar
}
