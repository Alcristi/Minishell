/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_double_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 19:33:46 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/02 22:01:16 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_strjoin_free1(char *s1, char const *s2, size_t size2)
{
	unsigned int	size;
	unsigned int	size1;
	char			*new;

	if (!s1 || !s2)
		return (NULL);
	size1 = ft_strlen(s1);
	size = size1 + size2 + 1;
	new = (char *)ft_calloc(sizeof(char), size);
	if (new == NULL)
		return (NULL);
	ft_strlcpy(new, s1, size1 + 1);
	ft_strlcpy(new + size1, s2, size2 + 1);
	ft_strdel(&s1);
	return (new);
}

static char	*ft_charjoin_free1(char *s1, char *s2)
{
	unsigned int	size;
	unsigned int	size1;
	char			*new;

	if (!s1 || !s2)
		return (NULL);
	size1 = ft_strlen(s1);
	size = size1 + 2;
	new = (char *)ft_calloc(sizeof(char), size);
	if (new == NULL)
		return (NULL);
	ft_strlcpy(new, s1, size1 + 1);
	ft_strlcpy(new + size1, s2, 2);
	ft_strdel(&s1);
	return (new);
}

static void	resolve_dolar_in_db_quotes(char **result, int *position, int *count)
{
	char	*temp;

	temp = NULL;
	temp = resolve_dollar(position);
	if (temp)
	{
		result[0] = ft_strjoin_free1(result[0], temp, ft_strlen(temp));
		count[0] += ft_strlen(temp) + 1;
		free(temp);
	}
}

char	*resolve_double_quotes(int *position)
{
	char	*result;
	int		count;
	char	aux;

	count = 0;
	result = ft_calloc(sizeof(char), ft_strlen(&g_core_var->buff[position[0]]));
	result[0] = '\0';
	if (g_core_var->buff[position[0]] == '\"')
	{
		position[0]++;
		while (g_core_var->buff[position[0]] != '\"')
		{
			if (g_core_var->buff[position[0]] == '$')
				resolve_dolar_in_db_quotes(&result, position, &count);
			else
			{
				aux = g_core_var->buff[position[0]];
				result = ft_charjoin_free1(result, &aux);
				count++;
				position[0]++;
			}
		}
	}
	return (result);
}
