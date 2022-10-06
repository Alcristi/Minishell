/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 02:24:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/06 03:02:43 by esilva-s         ###   ########.fr       */
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

static void	clear_aux(void)
{
	rl_clear_history();
	close(g_core_var->fd_stdin);
	close(g_core_var->fd_stdout);
	free_core();
}

int check_argument(char *str)
{
	int count;

	count = 0;
	if (str == NULL)
		return (1);
	while (count < ft_strlen(str))
	{
		if (ft_isdigit(str[count])
			|| str[count] == '+' || str[count] == '-')
			count++;
		else
			return (1);
	}
}

int	bt_exit(char **nb)
{
	int	nb_exit;
	int	count;

	count = 0;
	nb_exit = 0;
	printf("exit\n");
	if (check_args(nb) > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		//exit (1);
		return (1);
	}
	clear_aux();
	if (nb[1] != NULL)
	{
		while (nb[1][count] && (ft_isdigit(nb[1][count])
			|| nb[1][0] == '+' || nb[1][0] == '-'))
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
