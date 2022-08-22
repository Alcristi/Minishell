/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 23:30:03 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/22 23:07:16 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
static int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
*/

static int	is_pwd(char *str)
{
	int count;
	int size;

	count = 0;
	size = ft_strlen(str);
	if (size < 4)
		return (0);
	if (str[0] == 'P')
		count++;
	if (str[1] == 'W')
		count++;
	if (str[2] == 'D')
		count++;
	if (str[3] == '=')
		count++;
	if (count == 4)
		return (1);
	else
		return (0);
}

static char	*catch_pwd(char *data)
{
	int size;
	int	count;
	char *result;

	count = 0;
	size = ft_strlen(data);
	result = NULL;
	result = malloc(sizeof(char *) * size - 3);
	while (count + 4 < size)
	{
		result[count] = data[count + 4];
		count++;
	}
	result[count] = '\0';
	return (result);
}

int	bt_pwd(void)
{
	t_double_list	*temp;
	char			*result;

	result = NULL;
	temp = g_core_var->env;
	while (temp != NULL)
	{
		if (is_pwd(temp->data))
		{
			result = catch_pwd(temp->data);
			if (result != NULL)
			{
				printf("%s\n", result);
				free(result);
				return (0);
			}
		}
		else
			temp = temp->next;
	}
	return (1);
}