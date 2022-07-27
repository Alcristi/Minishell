/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_quots.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:55:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/27 09:18:47 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

static void convert_double_ll_in_string(t_double_list *result)
{
	int				total_characters;
	char			*tmp;
	t_double_list	*aux_result;

	aux_result = result;
	total_characters = 0;
	while(aux_result)
	{
		total_characters += ft_strlen(aux_result->data);
		aux_result = aux_result->next;
	}
	aux_result = result;
	tmp = ft_strdup("");
	while(aux_result)
	{
		tmp = ft_strjoin_gnl(tmp,aux_result->data);
		aux_result = aux_result->next;
	}
	free(g_core_var->buff);
	g_core_var->buff = ft_strdup(tmp);
	free(tmp);
}


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
			count--;
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
	convert_double_ll_in_string(result);
	free_list(result);
}
