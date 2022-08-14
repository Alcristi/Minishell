/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 11:55:21 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/12 01:23:16 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_core_var	*g_core_var;

static void	init_core(char **env)
{
	int	i;

	i = 0;
	g_core_var = (t_core_var *)malloc(sizeof(t_core_var));
	g_core_var->prompt.user = getenv("USER");
	g_core_var->confirm = 0;
	while (env[i])
	{
		if (i == 0)
			g_core_var->env = new (env[i]);
		else
			add_node_last(&g_core_var->env, env[i]);
		i++;
	}
}

static void	free_core(void)
{
	free(g_core_var->prompt.prompt);
	free(g_core_var->buff);
	free_list(g_core_var->env);
	free(g_core_var);
}

int	main(int argc, char *argv[], char *env[])
{
	int	i;

	i = 0;
	signal(SIGINT, sig_handle);
	init_core(env);
	prompt();
	rl_clear_history();
	free_core();
	return (0);
}
