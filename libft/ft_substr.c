/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:56:54 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/21 22:49:45 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs(int p1, int p2)
{
	if ((p1 - p2) < 0)
		return (p2 - p1);
	return (p1 - p2);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	size;
	size_t	i;
	char	*sub;

	if (!s)
		return (NULL);
	i = 0;
	size = ft_strlen(s);
	if ((ft_strlen(s + 1) < start))
		len = 0;
	if ((ft_strlen(s) + 1) < len + 1)
		sub = (char *)malloc(ft_strlen(s) + 1);
	else if (len == ft_strlen(s))
		sub = (char *)malloc(ft_abs((int)len, (int)start) + 1);
	else
		sub = (char *)malloc(len + 1);
	if (!sub)
		return (NULL);
	while ((i < len) && (start + i) < size)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
