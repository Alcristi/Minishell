/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 01:52:57 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/22 23:20:41 by esilva-s         ###   ########.fr       */
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
