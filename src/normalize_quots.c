/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_quots.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:55:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/26 00:31:31 by esilva-s         ###   ########.fr       */
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

static void	save_char(char caracter, t_double_list **result)
{
	char	*tmp;

	tmp = NULL;
	tmp = malloc(sizeof(char) + 1);
	tmp[0] = caracter;
	tmp[1] = '\0';
	add_node_last(&result[0], tmp);
	ft_strdel(&tmp);
}

static int	quotes_check_aux(int *count, t_double_list **result)
{
	char	*tmp;

	tmp = NULL;
	if (g_core_var->buff[*count] == '$')
	{
		tmp = resolve_dollar(count);
		if (tmp == NULL)
			return (1);
		add_node_last(result, tmp);
		count--;
	}
	else if (g_core_var->buff[*count] == '\'')
	{
		tmp = resolve_single_quotes(count);
		add_node_last(result, tmp);
	}
	else if (g_core_var->buff[*count] == '\"')
	{
		tmp = resolve_double_quotes(count);
		add_node_last(result, tmp);
	}
	ft_strdel(&tmp);
	return (0);
}

void	normalize_quotes(void)
{
	int				count;
	t_double_list	*result;

	count = 0;
	result = new("");
	while (g_core_var->buff[count])
	{
		if ((g_core_var->buff[count] == '$' && g_core_var->buff[count + 1])
			|| (g_core_var->buff[count] == '\'' && g_core_var->buff[count + 1])
			|| (g_core_var->buff[count] == '\"' && g_core_var->buff[count + 1]))
			quotes_check_aux(&count, &result);
		else
			save_char(g_core_var->buff[count], &result);
		count++;
	}
	convert_double_ll_in_string(result);
	free_list(result);
}
