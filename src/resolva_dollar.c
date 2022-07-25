/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 16:05:35 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/25 19:23:13 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ftx_isalnum(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)
		|| (c >= 48 && c <= 57) || c == 95)
		return (1);
	return (0);
}

static char	*cat_name_var(int count, char *buff)
{
	char	*temp;
	int		index;
	int		index2;

	index = 1;
	index2 = 0;
	while (buff[count + index])
	{
		if (buff[count + index] == ' ' || !ftx_isalnum(buff[count + index]))
			break ;
		index++;
	}
	temp = (char *) malloc(index * sizeof(char));
	while (index2 < index - 1)
	{
		temp[index2] = buff[count + index2 + 1];
		index2++;
	}
	temp[index2] = '\0';
	return (temp);
}

static int	search_var(char *var, char *env)
{
	int	count;
	int	index;

	count = 0;
	index = 0;
	while (env[index])
	{
		if (env[index] == '=')
			break ;
		index++;
	}
	if (index == ft_strlen(env))
		return (0);
	while (env[count] && var[count])
	{
		if (env[count] != var[count])
			return (0);
		count++;
	}
	if (count == index)
		return (1);
	else
		return (0);
}

static char	*cat_var(char *env, int len_name_var)
{
	int		size;
	int		count;
	char	*result;

	size = ft_strlen(env) - len_name_var;
	count = 0;
	result = malloc(size * sizeof(char));
	while (env[count + len_name_var + 1])
	{
		result[count] = env[count + len_name_var + 1];
		count++;
	}
	result[count] = '\0';
	return (result);
}

char	*resolve_dollar(int count)
{
	char			*name_var;
	char			*content_var;
	t_double_list	*aux_env;

	name_var = cat_name_var(count, g_core_var->buff);
	aux_env = g_core_var->env;
	content_var = NULL;
	while (aux_env != NULL)
	{
		if (search_var(name_var, aux_env->data))
		{
			content_var = cat_var(aux_env->data, ft_strlen(name_var));
			break ;
		}
		else
			aux_env = aux_env->next;
	}
	free(name_var);
	return (content_var);
}
