/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:56:36 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/21 22:50:56 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	index;

	if (!s)
		return (NULL);
	index = 0;
	str = ft_strdup(s);
	if (!str)
		return (NULL);
	while (str[index])
	{
		str[index] = f(index, str[index]);
		index++;
	}
	return (str);
}
