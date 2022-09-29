/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 11:55:21 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/29 11:56:53 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_core_var	*g_core_var;

static void	init_core(char **env)
{
	int	count;

	count = 0;
	g_core_var = (t_core_var *)ft_calloc(sizeof(t_core_var), 1);
	g_core_var->prompt.user = getenv("USER");
	g_core_var->fd_in = 0;
	g_core_var->fd_out = 0;
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
	init_core(env);
	prompt();
	rl_clear_history();
	free_core();
	exit (0);
}
