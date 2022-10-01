/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_string_aux.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 16:35:19 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/01 15:25:22 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	zerar(char **str, int size)
{
	int	count;

	count = 0;
	while (count < size)
	{
		str[0][count] = '\0';
		count++;
	}
}

static int	next_element(char **temp, t_double_list **aux, int nb)
{
	if (temp[0][0] != '\0')
	{
		add_node_last(&aux[0], temp[0]);
		zerar(temp, ft_strlen(temp[0]));
	}
	return (nb);
}

static t_vars	*init_and_destroy_vars(char c, int size, t_vars **vars)
{
	t_vars	*result;

	if (c == 'i')
	{
		result = ft_calloc(sizeof(t_vars), 2);
		result->temp = ft_calloc(sizeof(char), size + 1);
		result->quotes = 'n';
		result->index = 0;
		result->c_temp = 0;
		return (result);
	}
	else if (c == 'd')
	{
		ft_strdel(&vars[0]->temp);
		free(vars[0]);
		return (NULL);
	}
	else
		return (NULL);
}

static void	cut_aux(int option, t_vars *vars, char *str, t_double_list **aux)
{
	if (option == 1)
	{
		vars->quotes = 'n';
		vars->temp[vars->c_temp] = str[vars->index];
		vars->c_temp = next_element(&vars->temp, &aux[0], -1);
	}
	else if (option == 2)
	{
		vars->quotes = '\'';
		vars->c_temp = next_element(&vars->temp, &aux[0], 0);
		vars->temp[0] = str[vars->index];
	}
	else if (option == 3)
	{
		vars->quotes = 'n';
		vars->temp[vars->c_temp] = str[vars->index];
		vars->c_temp = next_element(&vars->temp, &aux[0], -1);
	}
	else if (option == 4)
	{
		vars->quotes = '\"';
		vars->c_temp = next_element(&vars->temp, &aux[0], 0);
		vars->temp[0] = str[vars->index];
	}
}

void	cut_quotes(char *str, t_double_list **aux)
{
	t_vars	*vars;

	vars = init_and_destroy_vars('i', ft_strlen(str), NULL);
	while (vars->index < ft_strlen(str))
	{
		if (str[vars->index] == '\'' && vars->quotes == '\'')
			cut_aux(1, vars, str, &aux[0]);
		else if (str[vars->index] == '\'' && vars->quotes == 'n')
			cut_aux(2, vars, str, &aux[0]);
		else if (str[vars->index] == '\"' && vars->quotes == '\"')
			cut_aux(3, vars, str, &aux[0]);
		else if (str[vars->index] == '\"' && vars->quotes == 'n')
			cut_aux(4, vars, str, &aux[0]);
		else
			vars->temp[vars->c_temp] = str[vars->index];
		vars->c_temp++;
		vars->index++;
	}
	if (vars->c_temp != 0)
		add_node_last(&aux[0], vars->temp);
	init_and_destroy_vars('d', 0, &vars);
}
