/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:22:16 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/23 18:38:27 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	str_prompt(void)
{
	char	*aux;

	init_dir(&g_core_var->prompt);
	aux = ft_strjoin(GREEN, g_core_var->prompt.user);
	g_core_var->prompt.prompt = ft_strjoin(aux, ": ");
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, END);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, BLUE);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, g_core_var->prompt.dir.current);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, END);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, "\e[1m$\e[0m ");
	free_dir(g_core_var);
	free(aux);
}

static int	check_exit(char *buff)
{
	if (buff == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	else if (!ft_strncmp(buff, "exit", ft_strlen("exit")))
		return (1);
	return (0);
}

static int	check_print(char *buff, t_double_list *env)
{
	if (!ft_strncmp(buff, "print", ft_strlen("print")))
	{
		print_linked_list(env);
		return (1);
	}
	return (0);
}

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

char	**build_cmd(t_stacks *stack, int id)
{
	t_token	*cursor;
	char	**arg_cmd;
	int		count_tokens;
	int		count;
	int		position_cmd;

	position_cmd = 0;
	count = 0;
	count_tokens = 0;
	while (1)
	{
		if (stack->stack_cmd->is_cmd)
		{
			position_cmd++;
			if (position_cmd > id)
				break ;
		}
		stack->stack_cmd = stack->stack_cmd->next;
	}
	cursor = stack->stack_cmd;
	while (cursor && !cursor->is_pipe)
	{
		count_tokens++;
		cursor = cursor->next;
	}
	arg_cmd = ft_calloc(sizeof(char *), count_tokens + 1);
	if (is_valid(stack->stack_cmd))
	{
		while (count < count_tokens && stack->stack_cmd)
		{
			arg_cmd[count] = ft_strdup(stack->stack_cmd->str);
			count++;
			if (!stack->stack_cmd->next)
			{
				free(stack->stack_cmd);
				stack->stack_cmd = NULL;
				break ;
			}
			stack->stack_cmd = stack->stack_cmd->next;
			if (stack->stack_cmd)
			{
				free(stack->stack_cmd->previus);
				stack->stack_cmd->previus = NULL;
			}
		}
		if (stack->stack_cmd && stack->stack_cmd->is_pipe)
		{
			stack->stack_cmd = stack->stack_cmd->next;
			free(stack->stack_cmd->previus);
			stack->stack_cmd->previus = NULL;
		}
		arg_cmd[count] = NULL;
		return (arg_cmd);
	}
	else
	{
		free(arg_cmd);
		return (NULL);
	}
}

void	child_process(t_stacks *stacks, int positon_cmd)
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
			g_core_var->fd_out = open(stacks->stack_out->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			g_core_var->fd_out = open(stacks->stack_out->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (g_core_var->fd_out < 0)
			exit(1);
	}
}

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
		while (1)
		{
			write(STDIN_FILENO, "> ", 2);
			line = get_next_line(STDIN_FILENO);
			if (!ft_strncmp(line, stacks->stack_herodoc->str, ft_strlen(stacks->stack_herodoc->str)))
			{
				free(line);
				free_stacks(&stacks);
				free_token(&tokens);
				free_core();
				exit(0);
			}
			write(fd_pp[1], line, ft_strlen(line));
			free(line);
		}
	}
	else
	{
		close(fd_pp[1]);
		dup2(fd_pp[0], STDIN_FILENO);
		wait(NULL);
	}
}

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

void	handle(int i)
{
	(void)i;
	write(0, "\n", 1);
	return ;
}

void	execute(t_stacks *stacks, t_token *tokens)
{
	pid_t	pid;
	int		status;
	char	**cmd;
	int		i;
	int size;

	size = amount_pipe(stacks);

	i = 0;

	pid = fork();
	signal(SIGINT, handle);
	if (pid == 0)
	{
		open_file(stacks);
		if (stacks->stack_herodoc)
			here_doc(stacks, tokens);
		for (i = 0; i < amount_pipe(stacks); i++)
			childProcess(stacks, i);
			i++;
		}
		cmd = build_cmd(stacks, i);
		dup(1);
		if (g_core_var->fd_out != 0)
			dup2(g_core_var->fd_out, STDOUT_FILENO);
		if(cmd)
			execve(cmd[0], cmd, g_core_var->envp);
		free_stacks(&stacks);
		free_token(&tokens);
		free_core();
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		//printf("exit with status: %d\n",WEXITSTATUS(status));
	}
}

void	prompt(void)
{
	t_token		*tokens;
	t_stacks	*stacks;

	while (1)
	{
		signal(SIGINT, sig_handle);
		str_prompt();
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		add_history(g_core_var->buff);
		if (check_exit(g_core_var->buff))
			break ;
		else if (!check_print(g_core_var->buff, g_core_var->env) && ft_strlen(g_core_var->buff) > 0)
		{
			normalize_quotes();
			tokens = tokenization_cmd(tokens);
			if (parse_tkn(tokens))
			{
				stacks = build_stack(tokens);
				execute(stacks, tokens);
				free_stacks(&stacks);
			}
		}
		if (ft_strlen(g_core_var->buff) > 0)
			free_token(&tokens);
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
	}
}
