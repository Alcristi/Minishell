/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 22:38:41 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/04 16:57:37 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	bt_echo(int nb, char **args)
{
	int	count;
	int	break_line;

	count = 0;
	break_line = 1;
	while (count < nb && args)
	{
		if (!ft_strcmp(args[count], "-n"))
			break_line = 0;
		else
			printf("%s", args[count]);
		count++;
		if (count < nb)
			printf(" ");
	}
	if (break_line == 1 || args == NULL)
		printf("\n");
	return (0);
}
