/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:55:08 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/17 16:55:09 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*aux_s;
	unsigned char	aux_c;
	size_t			i;

	i = 0;
	aux_c = (unsigned char) c;
	aux_s = (unsigned char *)s;
	while (i < n)
	{
		if (aux_s[i] == aux_c)
			return ((void *)(s + i));
		i++;
	}
	return (0);
}
