/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:22:16 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/14 21:02:43 by alcristi         ###   ########.fr       */
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

char argv[20][20] = {"/usr/bin/ls","/usr/bin/cat"};
char arg[20][20] = {"-l","-e"};

int count = -1;
void childProcess()
{
	count ++;

	pid_t	pid;
	int		fd[2];

	pipe(fd);

	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execl(argv[count],arg[count],NULL);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

void execute(t_stacks *stacks)
{
	pid_t pid;
	int	status;

	pid = fork();
	if (pid == 0)
	{
			for(int i = 0; i < 2; i++)
				childProcess();
			dup(1);
			execl("/usr/bin/ca","",NULL);
			free_stacks(&stacks);
			free(g_core_var->buff);
			free(g_core_var->prompt.prompt);
			exit(0);

	}
	else
	{
		waitpid(pid,&status,0);
		printf("exit with status: %d\n",WEXITSTATUS(status));
	}
}

void	prompt(void)
{
	t_token		*tokens;
	t_stacks	*stacks;

	while (1)
	{
		str_prompt();
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		add_history(g_core_var->buff);
		if (check_exit(g_core_var->buff))
			break ;
		else if (!check_print(g_core_var->buff, g_core_var->env))
		{
			normalize_quotes();
			printf("%s\n", g_core_var->buff);
			tokens = tokenization_cmd(tokens);
			if(parse_tkn(tokens))
			{
				stacks = build_stack(tokens);
				execute(stacks);
				free_stacks(&stacks);
			}
		}
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
		free_token(&tokens);
	}
}
