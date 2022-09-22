/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 02:24:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/22 19:19:03 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	bt_cd(char *path)
{
	int		result;

	if (path == NULL)
	{
		//printf();
		return (0);
	}
		
	result = 0;
	result = chdir(path);
	if (result == -1)
	{
		printf("cd: %s: No such file or directory\n", path);
		return (-1);
	}
	else
	{
		//free_dir(g_core_var);
		init_dir(&g_core_var->prompt);
	}
	return (0);
}
