/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:56:51 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/21 22:50:17 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_search_char(const char *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		i;

	if (!s1 || !set)
		return (NULL);
	while (*s1 && ft_search_char(set, *s1))
	{
		s1++;
	}
	i = ft_strlen(s1) - 1;
	while (i >= 0 && ft_search_char(set, s1[i]))
	{
		i--;
	}
	str = ft_substr(s1, 0, i + 1);
	return (str);
}
