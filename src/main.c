/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 11:55:21 by alcristi          #+#    #+#             */
/*   Updated: 2022/05/31 12:02:09 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include<readline/readline.h>
#include<readline/history.h>

t_core_var	*g_core_var;

int	ft_get_dir(char *str)
{
	int	size;

	size = ft_strlen(str) - 1;
	while (str[size] != '/')
		size--;
	return (size + 1);
}

void	init_dir(t_prompt *prompt)
{
	prompt->dir.path_current = getcwd(NULL, 0);
	prompt->dir.size_path = ft_strlen(g_core_var->prompt.dir.path_current);
	prompt->dir.position = ft_get_dir(g_core_var->prompt.dir.path_current);
	prompt->dir.current = ft_substr(g_core_var->prompt.dir.path_current,
			g_core_var->prompt.dir.position, g_core_var->prompt.dir.size_path);
}

void	free_dir(t_core_var *core)
{
	free(core->prompt.dir.path_current);
	free(core->prompt.dir.current);
}

void	str_prompt(void)
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

void	free_core(void)
{
	free(g_core_var->prompt.prompt);
	free(g_core_var->buff);
	free_list(g_core_var->env);
	free(g_core_var);
}

void	prompt(void)
{
	int	flag;

	flag = 1;
	while (flag)
	{
		str_prompt();
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		if (!ft_strncmp(g_core_var->buff, "exit", ft_strlen("exit")))
			break ;
		else if (!ft_strncmp(g_core_var->buff, "print", ft_strlen("print")))
			print_linked_list(g_core_var->env);
		add_history(g_core_var->buff);
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
	}
}

void	init_core(char **env)
{
	int	i;

	i = 0;
	g_core_var = (t_core_var *)malloc(sizeof(t_core_var));
	g_core_var->prompt.user = getenv("USER");
	while (env[i])
	{
		if (i == 0)
			g_core_var->env = new(env[i]);
		else
			add_node_last(&g_core_var->env, env[i]);
		i++;
	}
}

int	main(int argc, char *argv[], char *env[])
{
	int	i;

	i = 0;
	init_core(env);
	prompt();
	rl_clear_history();
	free_core();
	return (0);
}
