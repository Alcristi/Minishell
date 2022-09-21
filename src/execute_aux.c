/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:51:01 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/21 14:50:45 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	system_error(int number_err,char * str)
{
	char *error;

	if (number_err == INTERRUPT_SIG_INT)
		exit(INTERRUPT_SIG_INT);
	else if (number_err == CMD_NOT_FOUND)
	{

		error = ft_strjoin_gnl(str,": command not found\n");
		ft_putstr_fd(error,2);
		free(error);
		exit(CMD_NOT_FOUND);
	}
}

void check_cmd(char *str,t_stacks *stacks, t_token *tokens)
{
	struct stat info_stat;
	char	*error;

	stat(str,&info_stat);
	if (S_ISDIR(info_stat.st_mode))
	{
		error = ft_strjoin("Minishell: ",str);
		error = ft_strjoin_gnl(error,": is a directory\n");
		ft_putstr_fd(error,2);
		free(error);
		free_exec(stacks,tokens);
		exit (IS_DIRECTORY);
	}
	if(!access(str,F_OK))
	{
		if(access(str,X_OK))
		{
			error = ft_strjoin("Minishell:",str);
			error = ft_strjoin_gnl(error,": Permission denied\n");
			ft_putstr_fd(error,2);
			free(error);
			free_exec(stacks,tokens);
			exit(CMD_NOT_FOUND);
		}
	}
	else
	{
		error = ft_strjoin("Minishell:",str);
		error = ft_strjoin_gnl(error,": no such file or directory\n");
		ft_putstr_fd(error,2);
		free(error);
		free_exec(stacks,tokens);
		exit(PERMISSION_DENIED);
	}

}

int	is_valid(t_token *cmd,t_stacks *stacks, t_token *tokens)
{
	t_double_list	*aux_env;
	char			**tmp_path;
	int				count;
	char			*tmp;

	count = 1;
	if (ft_strchr(cmd->str,'/'))
	{
		check_cmd(cmd->str,stacks,tokens);
		return TRUE;
	}
	else
	{
		tmp = ft_strjoin("/", cmd->str);
		tmp_path = load_path();
		free(cmd->str);
		cmd->str = ft_strjoin(tmp_path[0], tmp);
		while (access(cmd->str, F_OK) && tmp_path[count])
		{
			free(cmd->str);
			cmd->str = ft_strjoin(tmp_path[count], tmp);
			count++;
		}
		free(tmp);
		free_double(tmp_path);
		if (access(cmd->str, F_OK))
		{
			tmp = ft_strdup(cmd->str);
			free_exec(stacks,tokens);

			system_error(CMD_NOT_FOUND,tmp);
		}
		return (1);
	}
}

static void	exit_child(t_stacks *stacks, t_token *tokens, char *line)
{
	free(line);
	free_stacks(&stacks);
	free_token(&tokens);
	free_core();
	exit(0);
}

// static void	heredoc_child(int fd_pp[2], t_stacks **stacks, t_token **tokens)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		write(STDIN_FILENO, "> ", 2);
// 		line = get_next_line(STDIN_FILENO);
// 		if (!ft_strncmp(line, stacks[0]->stack_herodoc->str,
// 				ft_strlen(stacks[0]->stack_herodoc->str)))
// 			exit_child(stacks[0], tokens[0], line);
// 		write(fd_pp[1], line, ft_strlen(line));
// 		free(line);
// 	}
// }

// void	here_doc(t_stacks *stacks, t_token *tokens)
// {
// 	int		fd_pp[2];
// 	pid_t	pid_hd;
// 	char	*line;

// 	if (pipe(fd_pp) == -1)
// 		exit(-1);
// 	pid_hd = fork();
// 	if (pid_hd == 0)
// 	{
// 		close(fd_pp[0]);
// 		heredoc_child(fd_pp, &stacks, &tokens);
// 	}
// 	else
// 	{
// 		close(fd_pp[1]);
// 		dup2(fd_pp[0], STDIN_FILENO);
// 		wait(NULL);
// 	}
// }

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
		while (1)
		{
			write(STDIN_FILENO, "> ", 2);
			line = get_next_line(STDIN_FILENO);
			if (!ft_strncmp(line, stacks->stack_herodoc->str,
					ft_strlen(stacks->stack_herodoc->str)))
			exit_child(stacks, tokens, line);
			if (is_priority == 2)
				write(fd_pp[1], line, ft_strlen(line));
			free(line);
		}
		close(fd_pp[1]);
	}
	else
	{
		close(fd_pp[1]);
		if(is_priority == 2)
			dup2(fd_pp[0], STDIN_FILENO);
		else
			close(fd_pp[0]);
		waitpid(pid_hd,&status,0);
		return (status);
	}
}
