/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 19:33:46 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/25 20:36:24 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_strjoin_free(char *s1, char const *s2)
{
	char	*conc;
	size_t	len;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	len = ft_strlen(s1) + ft_strlen(s2);
	conc = (char *)malloc(len + 1);
	if (!conc)
		return (NULL);
	while (i < ft_strlen(s1))
	{
		conc[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < ft_strlen(s2))
	{
		conc[i + ft_strlen(s1)] = s2[i];
		i++;
	}
	free (s1);
	conc[len] = '\0';
	return (conc);
}

char	*resolve_single_quotes(int *position)
{
	char	*result;
	int		count;

	count = 0;
	result = malloc(sizeof(char) * ft_strlen(&g_core_var->buff[position[0]]));
	if (g_core_var->buff[position[0]] == '\'')
	{
		position[0]++;
		while (g_core_var->buff[position[0]] != '\'')
		{
			result[count] = g_core_var->buff[position[0]];
			count++;
			position[0]++;
		}
		result[count] = '\0';
	}
	return (result);
}

char	*resolve_double_quotes(int *position)
{
	char	*result;
	char	*temp;
	int		count;

	count = 0;
	temp = NULL;
	result = malloc(sizeof(char) * ft_strlen(&g_core_var->buff[position[0]]));
	if (g_core_var->buff[position[0]] == '\"')
	{
		position[0]++;
		while (g_core_var->buff[position[0]] != '\"')
		{
			if (g_core_var->buff[position[0]] == '$')
			{
				temp = resolve_dollar(position);
				result = ft_strjoin_free(result, temp);
				count += ft_strlen(temp) + 1;
				free(temp);
			}
			else
			{
				result[count] = g_core_var->buff[position[0]];
				count++;
				position[0]++;
			}
		}
		result[count] = '\0';
	}
	return (result);
}
