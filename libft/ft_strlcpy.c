/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 00:22:06 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/17 16:56:27 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize)
{
	size_t	c;

	c = 0;
	if (destsize == 0)
		return (ft_strlen(src));
	while (c < destsize - 1)
	{
		if (src[c] == '\0')
		{
			dest[c] = '\0';
			break ;
		}
		else
		{
			dest[c] = src[c];
		}
		c++;
	}
	dest[c] = '\0';
	return (ft_strlen(src));
}
