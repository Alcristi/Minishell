/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:33:14 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/30 11:33:15 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	system_error(int number_err, char *str)
{
	char	*error;

	if (number_err == CMD_NOT_FOUND)
	{
		error = ft_strjoin_gnl(str, ": command not found\n");
		ft_putstr_fd(error, 2);
		free(error);
		exit(CMD_NOT_FOUND);
	}
}

void	validate_fork(int pid)
{
	if (pid == -1)
		exit(EXIT_FAILURE);
}

void	error_message(char *str, char *message_error)
{
	char		*error;

	error = ft_strjoin("Minishell: ", str);
	error = ft_strjoin_gnl(error, message_error);
	ft_putstr_fd(error, 2);
	free(error);
}
