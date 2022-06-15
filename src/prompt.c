/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:22:16 by alcristi          #+#    #+#             */
/*   Updated: 2022/06/15 18:58:14 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	t_token	*tokens;

	while (1)
	{
		str_prompt();
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		if (g_core_var->buff == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (!ft_strncmp(g_core_var->buff, "exit", ft_strlen("exit")))
			break ;
		else if (!ft_strncmp(g_core_var->buff, "print", ft_strlen("print")))
			print_linked_list(g_core_var->env);
		else
		{
			tokens = parse_cmd(tokens);
		}
		add_history(g_core_var->buff);
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
		free_token(tokens);
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
			g_core_var->env = new (env[i]);
		else
			add_node_last(&g_core_var->env, env[i]);
		i++;
	}
}
