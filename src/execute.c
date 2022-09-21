/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:53:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/21 15:23:05 by alcristi         ###   ########.fr       */
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

int is_builtin(t_stacks *cmd)
{
	if (!ft_strncmp(cmd->stack_cmd->str,"echo",ft_strlen("echo")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str,"cd",ft_strlen("cd")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str,"pwd",ft_strlen("pwd")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str,"export",ft_strlen("export")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str,"unset",ft_strlen("unset")))
		return (1);
	else if (!ft_strncmp(cmd->stack_cmd->str,"env",ft_strlen("env")))
		return (1);
	else
		return (0);
}

int exec_echo(char **cmd)
{
	int count_args;

	count_args = 0;
	while(cmd[count_args])
		count_args++;
	return (bt_echo(count_args -1,&cmd[1]));
}

int execute_builtin(char **cmd)
{
	if (!ft_strncmp(cmd[0],"echo",ft_strlen("echo")))
		return (exec_echo(cmd));
	else if (!ft_strncmp(cmd[0],"cd",ft_strlen("cd")))
		return (bt_cd(cmd[1]));
	else if (!ft_strncmp(cmd[0],"pwd",ft_strlen("pwd")))
		return (bt_pwd());
	else if (!ft_strncmp(cmd[0],"export",ft_strlen("export")))
		return (bt_export(cmd[1]));
	else if (!ft_strncmp(cmd[0],"unset",ft_strlen("unset")))
		return (bt_unset(cmd[1]));
	else if (!ft_strncmp(cmd[0],"env",ft_strlen("env")))
	{
		bt_env();
		return (0);
	}
}

int select_stdin(t_token *tokens)
{
	t_token *cursor;
	int	priority_stdin;

	priority_stdin = -1;
	cursor = tokens;
	while(cursor)
	{
		if (cursor->is_heredoc)
			priority_stdin = 2;
		else if (cursor->is_input)
			priority_stdin = 1;
		cursor = cursor->next;
	}
	return (priority_stdin);
}

void handle_redirect(t_stacks *stacks,t_token *tokens,int select_input, int pid)
{
	char *error;

	if (stacks->stack_herodoc && pid == 0)
	{
		g_core_var->exit_code =  here_doc(stacks,tokens,select_input);
		if (g_core_var->exit_code != 0)
		{
			g_core_var->exit_code = INTERRUPT_SIG_INT;
		}
	}

	if (stacks->stack_input && select_input == 1 && pid == 0)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
		{
			write(2,"Minisheel : ",13);
			ft_putstr_fd(stacks->stack_input->str,2);
			write(2,": ",2);
			perror(NULL);
			g_core_var->exit_code = EXIT_FAILURE;
		}
		dup2(g_core_var->fd_in, STDIN_FILENO);
	}

	if (stacks->stack_out && pid == 0)
	{
		if (stacks->stack_out->is_output)
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (g_core_var->fd_out < 0)
		{
			error = ft_strjoin("Minishell: ",stacks->stack_out->str);
			error = ft_strjoin_gnl(error,": Permission denied\n");
			ft_putstr_fd(error,2);
			free(error);
			g_core_var->exit_code = EXIT_FAILURE;
		}
	}
}

void free_exec(t_stacks *stacks, t_token *tokens)
{
	free_stacks(&stacks);
	free_token(&tokens);
	free_core();
}

void exec_cmd(t_stacks *stacks, t_token *tokens)
{
	int		priority_stdin;
	char	**cmd;
	pid_t	pid;
	int		status;

	priority_stdin = select_stdin(tokens);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
	{
		handle_redirect(stacks,tokens,priority_stdin,pid);
		cmd = build_cmd(stacks,tokens,0);
		if (g_core_var->fd_out != 0)
			dup2(g_core_var->fd_out, STDOUT_FILENO);
		if(cmd)
			execve(cmd[0], cmd, g_core_var->envp);
		perror(NULL);
		free_exec(stacks,tokens);
		exit(127);
	}
	else
	{
		waitpid (pid,&status,0);
		if (g_core_var->fd_in != 0)
			close(g_core_var->fd_in);
		if (g_core_var->fd_out != 0)
			close(g_core_var->fd_out);
	}
}

void handle_wait(int *pid)
{
	int count;

	count = 0;
	while(pid[count] != 0)
	{
		waitpid(pid[count],NULL,0);
		count++;
	}
}

void handle_pipe(int count, int quantity_cmd, int out_origin)
{
	int	std_out;
	int origin_stdout;

	origin_stdout = dup(out_origin);
	if (count  > 0)
	{
		dup2(g_core_var->fd_pipe[0],STDIN_FILENO);
		close(g_core_var->fd_pipe[0]);
	}

	if (count >= 0 && count != (quantity_cmd - 1) )
	{
		if (pipe(g_core_var->fd_pipe) == -1)
		{
			g_core_var->exit_code = EXIT_FAILURE;
			return ;
		}
		dup2(g_core_var->fd_pipe[1],STDOUT_FILENO);
		close(g_core_var->fd_pipe[1]);
	}
	else if (count == quantity_cmd - 1)
	{
		close(g_core_var->fd_pipe[0]);
		if (g_core_var->fd_out != 0)
		{
			dup2(g_core_var->fd_out,STDOUT_FILENO);
			close(g_core_var->fd_out);
		}
		else
			dup2(origin_stdout,STDOUT_FILENO);
	}
	close(origin_stdout);
}

void exec_with_pipe(t_stacks *stacks,t_token *tokens,int quantity_cmd)
{
	pid_t	pid;
	int		priority_stdin;
	char	**cmd;
	int		status;
	int		count;
	int		*pid_child;
	t_token *cursor;
	int input_origin;
	int out_ortigin;

	out_ortigin = dup(STDERR_FILENO);
	input_origin = dup(STDIN_FILENO);
	count = 0;
	priority_stdin = select_stdin(tokens);
	pid_child = ft_calloc(sizeof(int),quantity_cmd*2);
	cursor = stacks->stack_cmd;
	while (cursor)
	{
		pid_child[count] = fork();
		if (count == 0)
		{
			handle_redirect(stacks,tokens,priority_stdin,pid_child[count]);
			if (g_core_var->exit_code == INTERRUPT_SIG_INT || g_core_var->exit_code == EXIT_FAILURE && pid_child[count] == 0)
			{
				int exitc;

				exitc = g_core_var->exit_code;
				free(pid_child);
				close(out_ortigin);
				close(input_origin);
				free_exec(stacks,tokens);
				exit(exitc);
			}
		}

		handle_pipe(count,quantity_cmd,out_ortigin);
		if (pid_child[count] == -1)
			exit (10);
		if (pid_child[count] == 0)
		{
			close(input_origin);
			close(out_ortigin);
			close(g_core_var->fd_pipe[0]);
			close(g_core_var->fd_pipe[1]);
			free(pid_child);
			cmd = build_cmd(stacks,tokens,count);
			if(cmd)
				execve(cmd[0], cmd, g_core_var->envp);
			free_exec(stacks,tokens);
			perror(NULL);
			exit(127);
		}
		else
		{
			while (cursor && !cursor->is_pipe )
				cursor = cursor->next;
			if (cursor)
				cursor = cursor->next;
		}
		count++;
	}
	dup2(input_origin,STDIN_FILENO);
	if(g_core_var->fd_out != 0)
		dup2(out_ortigin,STDOUT_FILENO);
	handle_wait(pid_child);
	close(input_origin);
	close(out_ortigin);
	free(pid_child);
}

void exec_builtin(t_stacks *stacks, t_token *tokens)
{
	char **cmd;
	int		priority_stdin;
	int		copy_stdout;
	int		flag_redirect;

	flag_redirect = 0;
	copy_stdout = dup(STDOUT_FILENO);
	priority_stdin = select_stdin(tokens);
	handle_redirect(stacks,tokens,priority_stdin,0);
	cmd = build_cmd(stacks,tokens,0);
	if (g_core_var->fd_out != 0)
	{
		dup2(g_core_var->fd_out, STDOUT_FILENO);
		flag_redirect = 1;
	}
	execute_builtin(cmd);
	if(flag_redirect)
		dup2(copy_stdout, STDOUT_FILENO);
	free_double(cmd);
	close(copy_stdout);
	if (g_core_var->fd_out != 0)
		close(g_core_var->fd_out);
}

void	execute(t_stacks *stacks, t_token *tokens)
{
	int	quantity_pipe;

	quantity_pipe = amount_pipe(stacks);
	if(stacks->stack_cmd)
	{
		if (quantity_pipe)
			exec_with_pipe(stacks,tokens,quantity_pipe + 1);
		else
		{
			if (is_builtin(stacks))
				exec_builtin(stacks,tokens);
			else
				exec_cmd(stacks,tokens);
		}
	}
}
