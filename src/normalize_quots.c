/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_quots.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:55:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/22 16:53:38 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	normalize_quots()
{
	int count;
	char *result_result;
	t_double_list *result;

	count = 0;
	
	while (g_core_var->buff[count])
	{
		if (g_core_var->buff[count] == '$')
			add_node_last(&result, revolve_dollar(count))
		else if (g_core_var->buff[count] == '\'')
			add_node_last(&result, resolve_single_quots());
		else if (g_core_var->buff[count] == '\"')
			add_node_last(&result, revolve_double_quots());
		count++;
	}
}