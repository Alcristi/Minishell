/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 19:33:46 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/27 09:16:14 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	ft_strdel(char **pont)
{
	if (pont != NULL)
	{
		free(*pont);
		*pont = NULL;
	}
}

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
	int		count;
	char	aux;
	char	*temp;

	count = 0;
	result = malloc(sizeof(char) * ft_strlen(&g_core_var->buff[position[0]]));
	result[0] = '\0';
	if (g_core_var->buff[position[0]] == '\"')
	{
		position[0]++;
		while (g_core_var->buff[position[0]] != '\"')
		{
			if (g_core_var->buff[position[0]] == '$')
			{
				temp = NULL;
				temp = resolve_dollar(position);
				if (temp == NULL)
					continue;
				result = ft_strjoin_free1(result, temp, ft_strlen(temp));
				count += ft_strlen(temp) + 1;
				if (temp != NULL)
					free(temp);
			}
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
