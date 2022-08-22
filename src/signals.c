/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 00:31:23 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/18 17:22:36 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sig_handle(int signum)
{
	(void)signum;

	write(0,"\n",1);
	rl_replace_line("",0);
	rl_on_new_line();
	rl_redisplay();
}
