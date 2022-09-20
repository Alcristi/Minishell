/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_s_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 01:12:12 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/15 19:13:14 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*resolve_single_quotes(char *str)
{
	char	*result;
	int		index_r;
	int		index_s;

	index_r = 0;
	index_s = 0;
	result = ft_calloc(sizeof(char), ft_strlen(str) -1);
	while (str[index_s] != '\0')
	{
		if (str[index_s] != '\'')
		{
			result[index_r] = str[index_s];
			index_r++;
		}
		index_s++;
	}
	result[index_r] = '\0';
	return (result);
}
