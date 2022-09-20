/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 22:38:41 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/20 17:54:44 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	bt_echo(int nb, char **args)
{
	int	count;
	int	break_line;

	if (args == NULL)
		return (1);
	count = 0;
	break_line = 1;
	while (count < nb)
	{
		if (!ft_strcmp(args[count], "-n"))
			break_line = 0;
		else
			printf("%s ", args[count]);
		count++;
	}
	if (break_line == 1)
		printf("\n");
	return (0);
}
