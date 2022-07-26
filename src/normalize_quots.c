/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_quots.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:55:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/26 22:07:44 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	normalize_quotes(void)
{
	int				count;
	char			*result_result;
	t_double_list	*result;
	char			*tmp;

	count = 0;
	result = new("");
	while (g_core_var->buff[count])
	{
		if (g_core_var->buff[count] == '$' && g_core_var->buff[count + 1])
		{
			tmp = resolve_dollar(&count);
			if (tmp == NULL)
				continue ;
			add_node_last(&result, tmp);
		}
		else if (g_core_var->buff[count] == '\'' && g_core_var->buff[count + 1])
		{
			tmp = resolve_single_quotes(&count);
			add_node_last(&result, tmp);
		}
		else if (g_core_var->buff[count] == '\"' && g_core_var->buff[count + 1])
		{
			tmp = resolve_double_quotes(&count);
			add_node_last(&result, tmp);
		}
		else
		{
			tmp = malloc(sizeof(char) + 1);
			tmp[0] = g_core_var->buff[count];
			tmp[1] = '\0';
			add_node_last(&result, tmp);
		}
		free(tmp);
		count++;
	}
	print_linked_list(result);
	free_list(result);
}
