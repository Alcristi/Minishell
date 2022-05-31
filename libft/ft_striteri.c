/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:56:02 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/21 22:57:36 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_striteri(char const *s, void (*f)(unsigned int, char*))
{
	char	*str;
	size_t	index;

	if (!s)
		return (NULL);
	index = 0;
	str = (char *)s;
	while (str[index])
	{
		f(index, (str + index));
		index++;
	}
	return (str);
}
