/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 22:38:41 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/22 22:53:16 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
static int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
*/

static int	ft_strcmp(char *str, char *str2)
{
	int	size;
	int	count;

	size = ft_strlen(str);
	count = 0;
	if (size != ft_strlen(str2))
		return (1);
	while (count < size)
	{
		if (str[count] == str2[count])
			count++;
		else
			return (1);
	}
	return (0);
}

int	bt_echo(int nb, char **args)
{
	int	count;
	int break_line;

	if (args == NULL)
		return (1);
	count = 0;
	break_line = 1;
	while (count < nb)
	{
		if (!ft_strcmp(args[count], "-n"))
			break_line = 0;
		else
			printf("%s", args[count]);
		count++;
	}
	if (break_line == 1)
		printf("\n");
	return (0);
}

