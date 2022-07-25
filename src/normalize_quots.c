/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_quots.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:55:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/24 21:21:25 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *resolve_dolar(int *position)
{
	
}

static char *resolve_single_quotes(int *position)
{
	char *result;
	int	count;

	count = 0;
	result = malloc(sizeof(char) * ft_strlen(&g_core_var->buff[position[0]]));
	if(g_core_var->buff[position[0]] == '\'')
	{
		position[0]++;
		while(g_core_var->buff[position[0]] != '\'')
		{
			result[count] = g_core_var->buff[position[0]];
			count++;
			position[0]++;
		}
		result[count] = '\0';
	}
	return (result);
}

static char *resolve_double_quotes(int *position)
{
	char *result;
	int	count;

	count = 0;
	result = malloc(sizeof(char) * ft_strlen(&g_core_var->buff[position[0]]));
	if(g_core_var->buff[position[0]] == '\"')
	{
		position[0]++;
		while(g_core_var->buff[position[0]] != '\"')
		{
				result[count] = g_core_var->buff[position[0]];
				count++;
				position[0]++;
		}
		result[count] = '\0';
	}
	return (result);
}

void	normalize_quotes()
{
	int count;
	char *result_result;
	t_double_list *result;
	char *tmp;

	count = 0;
	result = new("");
	while (g_core_var->buff[count])
	{
		if (g_core_var->buff[count] == '$' && g_core_var->buff[count + 1])
		{
			tmp = resolve_single_quotes(&count);
			printf("%s\n",tmp);
			add_node_last(&result,tmp);
		}
		else if (g_core_var->buff[count] == '\'' && g_core_var->buff[count + 1])
		{
			tmp = resolve_single_quotes(&count);
			//printf("%s\n",tmp);
			add_node_last(&result,tmp);
		}
		else if (g_core_var->buff[count] == '\"' && g_core_var->buff[count + 1])
		{
			tmp = resolve_double_quotes(&count);
			printf("%s\n",tmp);
			add_node_last(&result,tmp);
		}
		else
		{
			tmp = malloc(sizeof(char) + 1);
			tmp[0] = g_core_var->buff[count];
			tmp[1] = '\0';
			add_node_last(&result,tmp);
		}
		free(tmp);
		count++;
	}
	print_linked_list(result);
	free_list(result);
}
