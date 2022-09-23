/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 19:13:28 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/22 17:14:58 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_signs(char *str, int size)
{
	int	count;

	count = 0;
	while (count < size)
	{
		if (str[count] == '\'')
			return (1);
		else if (str[count] == '\"')
			return (2);
		else if (str[count] == '$')
			return (3);
		count++;
	}
	return (0);
}

static char	*resolve_sign(char *str, int sign)
{
	char	*result;

	result = NULL;
	if (sign == 1)
		result = resolve_single_quotes(str);
	else if (sign == 2)
		result = resolve_double_quotes(str);
	else if (sign == 3)
		result = resolve_dollar(str);
	return (result);
}

int	resolve_string(t_token **tokens)
{
	char	*aux;
	char	*result;
	int		size;
	int		sign;

	aux = tokens[0]->str;
	result = NULL;
	size = ft_strlen(aux);
	sign = check_signs(aux, size);
	if (sign == 0)
		return (0);
	result = resolve_sign(aux, sign);
	if (result != NULL)
	{
		free(tokens[0]->str);
		if (sign == 1 || sign == 2)
			tokens[0]->quotes = TRUE;
		tokens[0]->str = ft_strdup(result);
		free(result);
	}
	else
		return (-1);
	return (1);
}
