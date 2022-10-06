/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 02:24:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/06 19:38:21 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	exit_error(char *nb)
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

static int	is_zero(char *str)
{
	size_t	count;

	count = 0;
	while (str[count] != '\0' && count < ft_strlen(str))
	{
		if (str[count] != '0' && str[count] != '-' && str[count] != '+')
			return (0);
		count++;
	}
	return (1);
}

static long	check_argument(char *str)
{
	size_t		count;
	long	exit_code;

	count = 0;
	while (count < ft_strlen(str))
	{
		if (ft_isdigit(str[count])
			|| str[count] == '+' || str[count] == '-' || str[count] == '\0')
			count++;
		else
		{
			exit_code = exit_error(str);
			return (exit_code);
		}
	}
	exit_code = ft_long_atoi(str);
	if (exit_code == 0)
	{
		if (!is_zero(str))
			exit_code = exit_error(str);
	}
	return (exit_code);
}

int	bt_exit(char **nb)
{
	long	nb_exit;
	int		count;

	count = 0;
	nb_exit = 0;
	printf("exit\n");
	if (check_args(nb) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	clear_aux();
	if (nb[1] != NULL)
	{
		nb_exit = check_argument(nb[1]);
		free_double(nb);
		exit(nb_exit);
	}
	free_double(nb);
	exit(0);
}
