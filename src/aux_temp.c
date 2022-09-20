/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_temp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 03:17:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/15 19:34:35 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	search_var(char *var, char *env)
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

char	*cat_var(char *env, int len_name_var)
{
	int		size;
	int		count;
	char	*result;

	size = ft_strlen(env) - len_name_var;
	count = 0;
	result = ft_calloc(sizeof(char), size);
	while (env[count + len_name_var + 1])
	{
		result[count] = env[count + len_name_var + 1];
		count++;
	}
	result[count] = '\0';
	return (result);
}
