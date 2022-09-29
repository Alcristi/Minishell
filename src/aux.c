/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 19:15:03 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/29 19:18:33 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_double(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*clear_quotes(char *str)
{
	char	**tmp;
	int		i;
	char	*aux;
	char	*text;

	i = 0;
	tmp = ft_split(str, '\"');
	while (tmp[i])
	{
		if (i == 0)
			text = ft_strjoin("", tmp[i]);
		else
		{
			aux = text;
			text = ft_strjoin(aux, tmp[i]);
			free(aux);
		}
		i++;
	}
	return (text);
}

char	*cut_str(char *str, char *to_find, int i)
{
	char	*s;

	if (str[i + ft_strlen(to_find)] != '\0')
	{
		s = ft_strdup((char *)&str[i + ft_strlen(to_find)]);
		str[i + ft_strlen(to_find)] = '\0';
	}
	else
	{
		s = ft_strdup((char *)&str[i]);
		str[i] = '\0';
	}
	return (s);
}

char	*ft_strchr_token(char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	if (c == '\0')
		return ((char *) s + ft_strlen(s));
	while (s[i])
	{
		if (s[i] == c)
		{
			if (s[i + 1] != '\0')
			{
				str = ft_strdup(&s[i + 1]);
				s[i + 1] = '\0';
			}
			else
			{
				str = ft_strdup(&s[i]);
				s[i] = '\0';
			}
			return (str);
		}
		i++;
	}
	return (0);
}

char	*ft_strnstr_token(char *str, char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (*to_find == '\0')
		return ((char *)str);
	while (str[i] && i < n)
	{
		j = 0;
		while (to_find[j] && str[j + i] && (to_find[j] == str[j + i])
			&& (j + i) < n)
			j++;
		if (to_find[j] == '\0')
			return (cut_str(str, to_find, i));
		i++;
	}
	return (0);
}
