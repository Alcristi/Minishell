/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_d_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 01:13:04 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/22 20:18:18 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	valid_name_char(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)
		|| (c >= 48 && c <= 57) || c == 95)
		return (1);
	return (0);
}

static int	check_dollar(char *str, int size)
{
	int	count;

	count = 0;
	while (count < size)
	{
		if (count == 0)
		{
			count++;
			continue ;
		}
		else if (str[count - 1] == '$' && valid_name_char(str[count]))
			return (1);
		count++;
	}
	return (0);
}

static char	*normalize_dollar(char *str)
{
	char	*temp;
	char	*result;

	result = NULL;
	result = ft_strdup(str);
	while (1)
	{
		if (check_dollar(result, ft_strlen(result)))
		{
			temp = resolve_dollar(result);
			if (temp != NULL)
			{
				free(result);
				result = ft_strdup(temp);
				free(temp);
			}
		}
		else
			break ;
	}
	return (result);
}

char	*resolve_double_quotes(char *str)
{
	char	*result;
	char	*aux;
	int		index_r;
	int		index_s;

	index_r = 0;
	index_s = 0;
	aux = normalize_dollar(str);
	result = ft_calloc(sizeof(char), ft_strlen(aux) -1);
	while (aux[index_s] != '\0')
	{
		if (aux[index_s] != '\"')
		{
			result[index_r] = aux[index_s];
			index_r++;
		}
		index_s++;
	}
	result[index_r] = '\0';
	free(aux);
	return (result);
}
