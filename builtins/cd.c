/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 02:24:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/05 23:19:30 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_args(char **path)
{
	int	count;

	count = 0;
	while (path[count])
		count++;
	return (count);
}

int	bt_cd(char **path)
{
	int		result;
	char	*error;

	if (path[1] == NULL)
		return (0);
	if (check_args(path) > 2)
	{
		ft_putstr_fd(" too many arguments", 2);
		return (1);
	}
	result = chdir(path[1]);
	if (result == -1)
	{
		error = ft_strjoin("cd: ", path[1]);
		error = ft_strjoin_gnl(error, ": No such file or directory\n");
		ft_putstr_fd(error, 2);
		free(error);
		return (1);
	}
	return (0);
}
