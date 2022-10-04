/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 02:24:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/04 17:42:49 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exit_error(char *nb)
{
	char	*str;

	str = ft_strjoin("minishell: exit: ", nb);
	str = ft_strjoin_gnl(str, ": numeric argument required\n");
	ft_putstr_fd(str, 2);
	free (str);
	str = NULL;
	return (2);
}

int	bt_exit(char **nb)
{
	int	nb_exit;
	int	count;

	count = 0;
	nb_exit = 0;
	rl_clear_history();
	close(g_core_var->fd_stdin);
	close(g_core_var->fd_stdout);
	free_core();
	if (check_args(nb) > 2)
	{
		ft_putstr_fd(" too many arguments",2);
		exit (1);
	}
	if (nb[1] != NULL)
	{
		while (nb[1][count] && (ft_isdigit(nb[1][count]) || nb[1][0] == '+' || nb[1][0] == '-'))
			count++;
		if (nb[1][count] != '\0')
			nb_exit = exit_error(nb[1]);
		else
			nb_exit = ft_atoi(nb[1]);
		free_double(nb);
		exit(nb_exit);
	}
	free_double(nb);
	exit(0);
}
