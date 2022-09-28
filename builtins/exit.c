/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 02:24:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/28 10:28:27 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	bt_exit(char *nb)
{
	int	nb_exit;

	nb_exit = 0;
	rl_clear_history();
	free_core();
	if (nb != NULL)
	{
		nb_exit = ft_atoi(nb);
		exit(nb_exit);
		return (nb_exit);
	}
	exit(0);
	return (0);
}
