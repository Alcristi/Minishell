/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 23:30:03 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/04 12:30:08 by alcristi         ###   ########.fr       */
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
	return (0);
}
