/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:28:04 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/03 22:25:13 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	convert_space_buff(void)
{
	int		size;
	int		count;
	char	quotes;

	count = 0;
	size = ft_strlen(g_core_var->buff);
	quotes = 'n';
	while (count < size)
	{
		if (g_core_var->buff[count] == '\'' && quotes == 'n')
			quotes == g_core_var->buff[count];
		else if (g_core_var->buff[count] == '\'' && quotes == '\'')
			quotes = 'n';
		else if (g_core_var->buff[count] == '\"' && quotes == '\"')
			quotes = 'n';
		else if (g_core_var->buff[count] == 32 && quotes != 'n')
			g_core_var->buff[count] = 7;
		count++;
	}
	return (quotes);
}
