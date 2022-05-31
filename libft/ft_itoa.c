/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:54:14 by alcristi          #+#    #+#             */
/*   Updated: 2021/08/19 18:40:03 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

static void	conv_str(int c, char *str, long int i)
{
	long int	n;

	n = c;
	if (c < 0)
	{
		str[0] = '-';
		n = -n;
	}
	if (n >= 10)
		conv_str((n / 10), str, (i - 1));
	str[i] = (n % 10) + '0';
}

char	*ft_itoa(int n)
{
	char		*str;
	long int	i;
	long int	len_int;

	i = n;
	len_int = 0;
	if (i <= 0)
		len_int++;
	while (i)
	{
		i = (i / 10);
		len_int++;
	}
	str = malloc(sizeof(char) * (len_int + 1));
	if (!str)
		return (NULL);
	str[len_int] = 0;
	conv_str(n, str, (len_int - 1));
	return (str);
}
