/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 19:15:03 by alcristi          #+#    #+#             */
/*   Updated: 2022/06/15 19:41:38 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_double(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*clear_quotes(char *str)
{
	char	**tmp;
	int		i;
	char	*aux;
	char	*text;

	i = 0;
	tmp = ft_split(str, '\"');
	while (tmp[i])
	{
		if (i == 0)
			text = ft_strjoin("", tmp[i]);
		else
		{
			aux = text;
			text = ft_strjoin(aux, tmp[i]);
			free(aux);
		}
		i++;
	}
	return (text);
}
