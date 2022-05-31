/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:56:48 by alcristi          #+#    #+#             */
/*   Updated: 2021/12/14 11:22:34 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

static int	letter_occurrence(const char *s, int c)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (s[i])
	{
		if (c == s[i])
			flag++;
		i++;
	}
	return (flag);
}

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	flag;
	int	checkflag;

	i = 0;
	flag = 0;
	checkflag = 0;
	if (c == '\0')
		return ((char *) s + ft_strlen(s));
	if (c > 256)
		c -= 256;
	flag = letter_occurrence(s, c);
	while (s[i])
	{
		if (s[i] == c)
			checkflag++;
		if (checkflag == flag && flag != 0)
			return ((char *) &s[i]);
		i++;
	}
	return (0);
}
