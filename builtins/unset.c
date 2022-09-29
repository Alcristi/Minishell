/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 19:30:37 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/29 18:53:31 by alcristi         ###   ########.fr       */
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
			return (0);
	}
	return (1);
}

static int	remove_env(t_double_list **env)
{
	t_double_list	*previus;
	t_double_list	*next;

	previus = NULL;
	next = NULL;
	if (env[0]->previus != NULL)
		previus = env[0]->previus;
	if (env[0]->next != NULL)
		next = env[0]->next;
	if (previus != NULL)
		previus->next = next;
	if (next != NULL)
		next->previus = previus;
	free(env[0]->data);
	free(env[0]);
	return (0);
}

int	bt_unset(char *name)
{
	t_double_list	*temp;

	if (name == NULL)
		return (1);
	temp = g_core_var->env;
	while (temp->previus != NULL)
		temp = temp->previus;
	while (temp != NULL)
	{
		if (search_var_bt(name, temp->data))
		{
			remove_env(&temp);
			return (1);
		}
		else
			temp = temp->next;
	}
	return (0);
}
