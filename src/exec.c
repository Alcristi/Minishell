/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:53:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/25 00:54:32 by esilva-s         ###   ########.fr       */
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

//auxilia a funcao execute a trabalhar com os processos filhos
static char	**child_aux(t_stacks **stacks, t_token **tokens)
{
	int		count;
	char	**cmd;

	count = 0;
	open_file(stacks[0]);
	if (stacks[0]->stack_herodoc)
		here_doc(stacks[0], tokens[0]);
	while (count < amount_pipe(stacks[0]))
	{
		child_process(stacks[0], count);
		count++;
	}
	cmd = build_cmd(stacks[0], count);
	return (cmd);
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
		cmd = child_aux(&stacks, &tokens);
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
