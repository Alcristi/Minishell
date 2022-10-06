/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_long_atoi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:48:08 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/06 19:30:42 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_size(unsigned long nb, int negative)
{
	if (negative == 1)
	{
		if (nb > 9223372036854775807U)
			return (1);
	}
	if (negative == -1)
	{
		if (nb > 9223372036854775808U)
			return (1);
	}
	return (0);
}

static int	ft_space(const char *c)
{
	if (*c == ' ' || *c == '\n' || *c == '\t')
		return (1);
	if (*c == '\v' || *c == '\f' || *c == '\r')
		return (1);
	return (0);
}

long	ft_long_atoi(const char *str)
{
	long	result;
	int		negative;

	negative = 1;
	result = 0;
	if (str == NULL)
		return (0);
	while (ft_space(str))
		str++;
	if (*str == '-')
		negative = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str && *str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - 48);
		str++;
	}
	if (check_size(result, negative))
		return (0);
	result = result * negative;
	return (result);
}
