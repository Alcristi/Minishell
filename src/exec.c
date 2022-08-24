/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:53:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/24 01:57:37 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//verifica nos caminhos do $PATH se existe o comando solicitado
int	is_valid(t_token *cmd)
{
	t_double_list	*aux_env;
	char			*path;
	char			**tmp_path;
	int				count;
	char			*tmp;

	count = 1;
	aux_env = g_core_var->env;
	tmp = ft_strjoin("/", cmd->str);
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
	tmp_path = ft_split(path, ':');
	free(cmd->str);
	cmd->str = ft_strjoin(tmp_path[0], tmp);
	while (access(cmd->str, F_OK) && tmp_path[count])
	{
		free(cmd->str);
		cmd->str = ft_strjoin(tmp_path[count], tmp);
		count++;
	}
	free(path);
	free(tmp);
	free_double(tmp_path);
	if (access(cmd->str, F_OK))
		return (0);
	return (1);
}

//executa os comandos nos processos netos
static void	child_process(t_stacks *stacks, int positon_cmd)
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
		execve(cmd[0], cmd, g_core_var->envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

//abre os arquivos de entrada e de saida
static void	open_file(t_stacks *stacks)
{
	if (stacks->stack_input)
	{
		g_core_var->fd_in = open(stacks->stack_input->str, O_RDONLY);
		if (g_core_var->fd_in < 0)
			exit(1);
		dup2(g_core_var->fd_in, 0);
	}
	if (stacks->stack_out)
	{
		if (stacks->stack_out->is_output)
			g_core_var->fd_out = open(stacks->stack_out->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			g_core_var->fd_out = open(stacks->stack_out->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (g_core_var->fd_out < 0)
			exit(1);
	}
}

static void	child_aux(t_stacks *stacks, t_token *tokens, char **cmd)
{
	int		count;

	count = 0;
	open_file(stacks);
	if (stacks->stack_herodoc)
		here_doc(stacks, tokens);
	while (count < amount_pipe(stacks))
	{
		child_process(stacks, count);
		count++;
	}
	cmd = build_cmd(stacks, count);
}

//executa os comando passados no prompt atravez das stacks
void	execute(t_stacks *stacks, t_token *tokens)
{
	pid_t	pid;
	int		status;
	char	**cmd;
	int		size;
	int		count;

	count = 0;
	size = amount_pipe(stacks);
	pid = fork();
	signal(SIGINT, handle);
	if (pid == 0)
	{
		//child_aux(stacks, tokens, cmd);
		open_file(stacks);
		if (stacks->stack_herodoc)
			here_doc(stacks, tokens);
		while (count < amount_pipe(stacks))
		{
			child_process(stacks, count);
			count++;
		}
		cmd = build_cmd(stacks, count);
		dup(1);
		if (g_core_var->fd_out != 0)
			dup2(g_core_var->fd_out, STDOUT_FILENO);
		if (cmd)
			execve(cmd[0], cmd, g_core_var->envp);
		free_stacks(&stacks);
		free_token(&tokens);
		free_core();
		exit(0);
	}
	else
		waitpid(pid, &status, 0);
}
//printf("exit with status: %d\n",WEXITSTATUS(status));