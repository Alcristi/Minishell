/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 22:05:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/07 00:33:05 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

int	ft_strcmp(char *str, char *str2)
{
	size_t	size;
	size_t	count;

	size = ft_strlen(str);
	count = 0;
	if (size != ft_strlen(str2))
		return (1);
	while (count < size)
	{
		if (str[count] == str2[count])
			count++;
		else
			return (1);
	}
	return (0);
}
