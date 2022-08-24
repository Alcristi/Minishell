/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 11:55:21 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/24 01:11:38 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_core_var	*g_core_var;

static void	init_core(char **env)
{
	int	count;

	count = 0;
	g_core_var = (t_core_var *)malloc(sizeof(t_core_var));
	g_core_var->prompt.user = getenv("USER");
	g_core_var->confirm = 0;
	g_core_var->fd_in = 0;
	g_core_var->fd_out = 0;
	g_core_var->envp = env;
	while (env[count])
	{
		if (count == 0)
			g_core_var->env = new (env[count]);
		else
			add_node_last(&g_core_var->env, env[count]);
		count++;
	}
}

void	free_core(void)
{
	free(g_core_var->prompt.prompt);
	free(g_core_var->buff);
	free_list(g_core_var->env);
	free(g_core_var);
}

int	main(int argc, char *argv[], char *env[])
{
	init_core(env); //OK
	prompt(); //---
	rl_clear_history();
	free_core();
	return (0);
}
