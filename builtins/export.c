/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 19:48:41 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/22 20:23:57 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	verify_name(char *name)
{
	int	count;

	if (name == NULL)
		return (1);
	count = 0;
	if (!ft_isalpha(name[count]))
		return (1);
	while (name[count] != '\0')
	{
		if (!ft_isalnum(name[count]))
			return (1);
		count++;
	}
	return (0);
}

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
	if (pos_equal == 0)
		return (NULL);
	name = malloc(sizeof(char) * pos_equal + 1);
	while (count < pos_equal)
	{
		name[count] = arg[count];
		count++;
	}
	name[count] = '\0';
	return (name);
}

/*
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
}*/

static void	print_vars(void)
{
	t_double_list	*aux;

	aux = g_core_var->env;
	while (aux->previus != NULL)
		aux = aux->previus;
	while (aux != NULL)
	{
		printf("declare -x %s\n", aux->data);
		aux = aux->next;
	}
}

int	localize_var(char *name_var, char *arg)
{
	t_double_list	*aux_env;

	aux_env = g_core_var->env;
	while (aux_env->previus != NULL)
		aux_env = aux_env->previus;
	while (aux_env != NULL)
	{
		if (search_var(name_var, aux_env->data))
		{
			free(aux_env->data);
			aux_env->data = ft_strdup(arg);
			return (1);
		}
		else
			aux_env = aux_env->next;
	}
	return (0);
}

int	bt_export(char *arg)
{
	char	*name;

	if (arg == NULL)
	{
		print_vars();
		return (0);
	}
	name = catch_name(arg);
	if (name == NULL || verify_name(name))
	{
		printf("`%s\': not a valid identifier\n", arg);
		return (1);
	}
	if (localize_var(name, arg))
	{
		free(name);
		return (0);
	}
	add_node_last(&g_core_var->env, arg);
	free(name);
	return (0);
}
