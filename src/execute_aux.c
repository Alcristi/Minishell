/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:51:01 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/02 01:57:04 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//abre os arquivos de entrada e de saida
void	open_file(t_stacks *stacks)
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
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			g_core_var->fd_out = open(stacks->stack_out->str,
					O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (g_core_var->fd_out < 0)
			exit(1);
	}
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

static void	exit_child(t_stacks *stacks, t_token *tokens, char *line)
{
	free(line);
	free_stacks(&stacks);
	free_token(&tokens);
	free_core();
	exit(0);
}

static void	heredoc_child(int fd_pp[2], t_stacks **stacks, t_token **tokens)
{
	char	*line;

	while (1)
	{
		write(STDIN_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, stacks[0]->stack_herodoc->str,
				ft_strlen(stacks[0]->stack_herodoc->str)))
			exit_child(stacks[0], tokens[0], line);
		write(fd_pp[1], line, ft_strlen(line));
		free(line);
	}
}

//abre um arquivo de entrada na entrada padrão
void	here_doc(t_stacks *stacks, t_token *tokens)
{
	int		fd_pp[2];
	pid_t	pid_hd;
	char	*line;

	if (pipe(fd_pp) == -1)
		exit(-1);
	pid_hd = fork();
	if (pid_hd == 0)
	{
		close(fd_pp[0]);
		heredoc_child(fd_pp, &stacks, &tokens);
	}
	else
	{
		close(fd_pp[1]);
		dup2(fd_pp[0], STDIN_FILENO);
		wait(NULL);
	}
}
