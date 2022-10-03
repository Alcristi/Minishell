/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 00:31:23 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/03 21:36:02 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sig_handle(int signum)
{
	(void)signum;
	write(0, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle(int i)
{
	(void)i;
	write(0, "\n", 1);
	return ;
}

void	handle_here(int i)
{
	(void)i;
	write(0, "\n", 1);
	exit (130);
}

void	handle_quit(int i)
{
	(void)i;
	ft_putstr_fd("Quit\n", 2);
	return ;
}

void	init_point(char ***cmd, char ***envp)
{
	cmd[0] = NULL;
	envp[0] = NULL;
}
