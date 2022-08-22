/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 19:30:37 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/22 23:06:54 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	search_var_bt(char *name_var, char *data)
{
	int	count;
	int	correct;

	count = 0;
	correct = 1;
	if (name_var == NULL || data == NULL)
		return (0);
	if (ft_strlen(name_var) > ft_strlen(data))
		return (0);
	while (count < ft_strlen(name_var))
	{
		if (name_var[count] == data[count])
			count++;
		else
			return(0);
	}
	return (1);
}

static int	remove_env(t_double_list *env)
{
	t_double_list	*previus;
	t_double_list	*next;

	previus = env->previus;
	next = env->next;
	previus->next = next;
	next->previus = previus;
	free(env);
	return (0);
}

int	bt_unset(char *name)
{
	t_double_list *temp;

	if (name == NULL)
		return (1);
	temp = g_core_var->env;
	while (temp->previus != NULL)
		temp = temp->previus;
	while (temp != NULL)
	{
		if (search_var_bt(name, temp->data))
		{
			remove_env(temp);
			return (1);
		}
		else
			temp = temp->next;
	}
	return (0);
}