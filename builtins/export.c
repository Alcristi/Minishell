/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 19:48:41 by esilva-s          #+#    #+#             */
/*   Updated: 2022/08/22 23:20:05 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*catch_name(char *arg)
{
	char	*name;
	int		pos_equal;
	int		count;

	pos_equal = 0;
	count = 0;
	name = NULL;
	while (pos_equal < ft_strlen(arg) && arg[pos_equal] != '=')
		pos_equal++;
	if (pos_equal >= ft_strlen(arg))
		return (NULL);
	name = malloc(sizeof(char) * pos_equal);
	while (count < pos_equal)
	{
		name[count] = arg[count];
		count++;
	}
	name[count] = '\0';
	return (name);
}

static char	*catch_content(char *arg)
{
	char	*content;
	int		count;
	int		pos_equal;

	content = NULL;
	count = 0;
	pos_equal = 0;
	while (pos_equal < ft_strlen(arg) && arg[pos_equal] != '=')
		pos_equal++;
	content = malloc(sizeof(char) * ft_strlen(arg) - pos_equal);
	while (count < ft_strlen(arg) - pos_equal - 1)
	{
		content[count] = arg[count + pos_equal + 1];
		count++;
	}
	content[count] = '\0';
	return (content);
}

int	bt_export(char *arg)
{
	char	*name;
	char	*content;

	name = catch_name(arg);
	if (name == NULL)
		return (1);
	content = catch_content(arg);
	if (content == NULL)
	{
		free(name);
		return (1);
	}
	add_node_last(g_core_var->env, arg);
	free(name);
	free(content);
	return (0);
}
