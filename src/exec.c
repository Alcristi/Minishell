/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:53:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/07 20:53:07 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**load_path(void)
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
int	is_valid(t_token *cmd)
{
	t_double_list	*aux_env;
	char			**tmp_path;
	int				count;
	char			*tmp;

	count = 1;
	if(ft_strchr(cmd->str,'/'))
		return (!access(cmd->str, F_OK | X_OK));
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
		return (0);
	return (1);
}

int is_cmd_builtin(char **cmd)
{
	if (!ft_strncmp(cmd[0],"echo",ft_strlen("echo")))
		return (1);
	else if (!ft_strncmp(cmd[0],"cd",ft_strlen("cd")))
		return (1);
	else if (!ft_strncmp(cmd[0],"pwd",ft_strlen("pwd")))
		return (1);
	else if (!ft_strncmp(cmd[0],"export",ft_strlen("export")))
		return (1);
	else if (!ft_strncmp(cmd[0],"unset",ft_strlen("unset")))
		return (1);
	else if (!ft_strncmp(cmd[0],"env",ft_strlen("env")))
		return (1);
	else
		return (0);
}

int exec_builtin(char **cmd)
{
	if (!ft_strncmp(cmd[0],"echo",ft_strlen("echo")))
	{
		char	**echo_args;
		int		argc;
		return (bt_echo(1,&cmd[1]));
	}
	else if (!ft_strncmp(cmd[0],"cd",ft_strlen("cd")))
		return (0);
	else if (!ft_strncmp(cmd[0],"pwd",ft_strlen("pwd")))
		return (bt_pwd());
	else if (!ft_strncmp(cmd[0],"export",ft_strlen("export")))
	{
		return (bt_export(cmd[1]));
	}
	else if (!ft_strncmp(cmd[0],"unset",ft_strlen("unset")))
	{
		return (bt_unset(cmd[1]));
	}
	else if (!ft_strncmp(cmd[0],"env",ft_strlen("env")))
	{
		bt_env();
		return (0);
	}
	else
		return (0);
}

//executa os comandos nos processos netos
static void	child_process(t_stacks *stacks, int positon_cmd, t_token *tokens)
{
	pid_t	pid;
	int		fd[2];
	char	**cmd;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		cmd = build_cmd(stacks, positon_cmd);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (cmd && is_cmd_builtin(cmd))
			exec_builtin(cmd);
		else if(cmd)
			execve(cmd[0], cmd, g_core_var->envp);
		free_stacks(&stacks);
		free_token(&tokens);
		if (cmd)
			free_double(cmd);
		free_core();
		exit(0);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
	}
}

//auxilia a funcao execute a trabalhar com os processos filhos
static char	**child_aux(t_stacks **stacks, t_token **tokens)
{
	char	**cmd;
	int		count;
	int		size;

	count = 0;
	size = amount_pipe(stacks[0]);
	cmd = NULL;
	open_file(stacks[0]);
	if (stacks[0]->stack_herodoc)
		here_doc(stacks[0], tokens[0]);
	while ( stacks[0]->stack_cmd && count < size)
	{
		child_process(stacks[0], count,tokens[0]);
		count++;
	}
	if(stacks[0]->stack_cmd)
		cmd = build_cmd(stacks[0], count);
	return (cmd);
}

//executa os comando passados no prompt atravez das stacks

void	execute(t_stacks *stacks, t_token *tokens)
{
	pid_t	pid;
	int		status;
	char	**cmd;
	int		count;

	count = 0;
	cmd = NULL;
	pid = fork();
	signal(SIGINT, handle);
	if (pid == 0)
	{
		cmd = child_aux(&stacks, &tokens);
		dup(1);
		if (g_core_var->fd_out != 0)
			dup2(g_core_var->fd_out, STDOUT_FILENO);
		if (cmd && is_cmd_builtin(cmd))
			status = exec_builtin(cmd);
		else if(cmd)
			status = execve(cmd[0], cmd, g_core_var->envp);
		free_stacks(&stacks);
		free_token(&tokens);
		if (cmd)
			free_double(cmd);
		free_core();
		exit(0);
	}
	else
		waitpid(pid, &status, 0);
}
