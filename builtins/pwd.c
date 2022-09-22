/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 23:30:03 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/22 22:44:43 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	bt_pwd(void)
{
	char	*path_current;

	path_current = NULL;
	path_current = getcwd(NULL, 0);
	printf("%s\n", path_current);
	if (path_current != NULL)
		free(path_current);
	return (1);
}
