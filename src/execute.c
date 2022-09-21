/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:53:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/21 02:26:09 by esilva-s         ###   ########.fr       */
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

//verifica nos caminhos do $PATH se existe o comando solicitado
// int	is_valid(t_token *cmd)
// {
// 	t_double_list	*aux_env;
// 	char			**tmp_path;
// 	int				count;
// 	char			*tmp;

// 	count = 1;
// 	if(ft_strchr(cmd->str,'/'))
// 		return (!access(cmd->str, F_OK | X_OK));
// 	tmp = ft_strjoin("/", cmd->str);
// 	tmp_path = load_path();
// 	free(cmd->str);
// 	cmd->str = ft_strjoin(tmp_path[0], tmp);
// 	while (access(cmd->str, F_OK) && tmp_path[count])
// 	{
// 		free(cmd->str);
// 		cmd->str = ft_strjoin(tmp_path[count], tmp);
// 		count++;
// 	}
// 	free(tmp);
// 	free_double(tmp_path);
// 	if (access(cmd->str, F_OK))
// 		return (0);
// 	return (1);
// }


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
	//eu adicionei esse return pois estava apitando na copilação
}

// int is_cmd_builtin(char **cmd)
// {
// 	if (!ft_strncmp(cmd[0],"echo",ft_strlen("echo")))
// 		return (1);
// 	else if (!ft_strncmp(cmd[0],"cd",ft_strlen("cd")))
// 		return (1);
// 	else if (!ft_strncmp(cmd[0],"pwd",ft_strlen("pwd")))
// 		return (1);
// 	else if (!ft_strncmp(cmd[0],"export",ft_strlen("export")))
// 		return (1);
// 	else if (!ft_strncmp(cmd[0],"unset",ft_strlen("unset")))
// 		return (1);
// 	else if (!ft_strncmp(cmd[0],"env",ft_strlen("env")))
// 		return (1);
// 	else
// 		return (0);
// }

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
	return (-1);
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

void handle_redirect(t_stacks *stacks,t_token *tokens,int select_input)
{
	if (stacks->stack_herodoc)
	{
		g_core_var->exit_code =  here_doc(stacks,tokens,select_input);
		if (g_core_var->exit_code != 0)
			exit (130);
	}

	if (stacks->stack_input && select_input == 1)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
		{
			write(2,"Minisheel : ",13);
			ft_putstr_fd(stacks->stack_input->str,2);
			write(2,": ",2);
			perror(NULL);
			g_core_var->exit_code = EXIT_FAILURE;
			exit(EXIT_FAILURE);
		}
		dup2(g_core_var->fd_in, STDIN_FILENO);
	}

	if (stacks->stack_out)
	{
		if (stacks->stack_out->is_output)
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (g_core_var->fd_out < 0)
		{
			perror(NULL);
			g_core_var->exit_code = EXIT_FAILURE;
			exit(EXIT_FAILURE);
		}
	}
}

void free_exec(t_stacks *stacks, t_token *tokens)
{
	free_token(&tokens);
	free_stacks(&stacks);
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
		handle_redirect(stacks,tokens,priority_stdin);
		cmd = build_cmd(stacks,0);
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

void exec_with_pipe(t_stacks *stacks, t_token *tokens)
{
	int		priority_stdin;
	char	**cmd;
	pid_t	pid;
	int		status;

	priority_stdin = select_stdin(tokens);
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
	handle_redirect(stacks,tokens,priority_stdin);
	cmd = build_cmd(stacks,0);
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
	if(stacks->stack_cmd)
	{
		if (amount_pipe(stacks))
			exec_with_pipe(stacks,tokens);
		else
		{
			if (is_builtin(stacks))
				exec_builtin(stacks,tokens);
			else
				exec_cmd(stacks,tokens);
		}
	}
}
