/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 01:52:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/19 23:43:32 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	bt_env(void)
{
	t_double_list	*temp;

	temp = g_core_var->env;
	while (temp != NULL)
	{
		printf("%s\n", temp->data);
		temp = temp->next;
	}
}
