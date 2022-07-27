/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_quots.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:55:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/26 23:22:50 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	convert_double_ll_in_string(t_double_list *result)
{
	int				total_characters;
	char			*tmp;
	t_double_list	*aux_result;

	aux_result = result;
	total_characters = 0;
	while (aux_result)
	{
		total_characters += ft_strlen(aux_result->data);
		aux_result = aux_result->next;
	}
	aux_result = result;
	tmp = ft_strdup("");
	while (aux_result)
	{
		tmp = ft_strjoin_gnl(tmp, aux_result->data);
		aux_result = aux_result->next;
	}
	free(g_core_var->buff);
	g_core_var->buff = ft_strdup(tmp);
	free(tmp);
}

char	*load_single_char(char c, int position)
{
	char	*str;

	str = malloc(sizeof(char) + 1);
	str[0] = g_core_var->buff[position];
	str[1] = '\0';
	return (str);
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
			tmp = resolve_dollar(&count);
		else if (g_core_var->buff[count] == '\'' && g_core_var->buff[count + 1])
			tmp = resolve_single_quotes(&count);
		else if (g_core_var->buff[count] == '\"' && g_core_var->buff[count + 1])
			tmp = resolve_double_quotes(&count);
		else
			tmp = load_single_char(g_core_var->buff[count], count);
		if (tmp == NULL)
			continue ;
		add_node_last(&result, tmp);
		free(tmp);
		count++;
	}
	convert_double_ll_in_string(result);
	printf("%s\n",g_core_var->buff);
	free_list(result);
}
