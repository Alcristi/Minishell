/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:28:04 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/08 18:28:39 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	convert_space_buff(void)
{
	int	size;
	int	count;
	int	active_s_quotes;
	int	active_d_quotes;

	count = 0;
	active_s_quotes = 0;
	active_d_quotes = 0;
	size = ft_strlen(g_core_var->buff);
	while (count < size)
	{
		if (g_core_var->buff[count] == '\'' && active_s_quotes == 0)
			active_s_quotes = 1;
		else if (g_core_var->buff[count] == '\"' && active_d_quotes == 0)
			active_d_quotes = 1;
		else if (g_core_var->buff[count] == '\'' && active_s_quotes == 1)
			active_s_quotes = 0;
		else if (g_core_var->buff[count] == '\"' && active_d_quotes == 1)
			active_d_quotes = 0;
		else if (g_core_var->buff[count] == 32
			&& (active_s_quotes || active_d_quotes))
			g_core_var->buff[count] = 7;
		count++;
	}
}
