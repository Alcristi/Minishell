/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_single_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 10:57:32 by alcristi          #+#    #+#             */
/*   Updated: 2022/07/27 10:57:33 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*resolve_single_quotes(int *position)
{
	char	*result;
	int		count;

	count = 0;
	result = malloc(sizeof(char) * ft_strlen(&g_core_var->buff[position[0]]));
	if (g_core_var->buff[position[0]] == '\'')
	{
		position[0]++;
		while (g_core_var->buff[position[0]] != '\'')
		{
			result[count] = g_core_var->buff[position[0]];
			count++;
			position[0]++;
		}
		result[count] = '\0';
	}
	return (result);
}
