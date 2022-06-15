/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:58:21 by alcristi          #+#    #+#             */
/*   Updated: 2022/06/15 18:58:22 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_get_dir(char *str)
{
	int	size;

	size = ft_strlen(str) - 1;
	while (str[size] != '/')
		size--;
	return (size + 1);
}

void	init_dir(t_prompt *prompt)
{
	prompt->dir.path_current = getcwd(NULL, 0);
	prompt->dir.size_path = ft_strlen(g_core_var->prompt.dir.path_current);
	prompt->dir.position = ft_get_dir(g_core_var->prompt.dir.path_current);
	prompt->dir.current = ft_substr(g_core_var->prompt.dir.path_current,
			g_core_var->prompt.dir.position, g_core_var->prompt.dir.size_path);
}

void	free_dir(t_core_var *core)
{
	free(core->prompt.dir.path_current);
	free(core->prompt.dir.current);
}
