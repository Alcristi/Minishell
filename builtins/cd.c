/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 02:24:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/28 10:28:17 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	bt_cd(char *path)
{
	int		result;
	char	*aux;

	if (path == NULL)
	{
		return (0);
	}
	result = 0;
	result = chdir(path);
	if (result == -1)
	{
		printf("cd: %s: No such file or directory\n", path);
		return (-1);
	}
	return (0);
}
