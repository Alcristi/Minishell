/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_dollar_aux.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 19:32:07 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/15 19:34:59 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*catch_var(char *name_var)
{
	t_double_list	*aux_env;
	char			*content_var;

	aux_env = g_core_var->env;
	while (aux_env->previus != NULL)
		aux_env = aux_env->previus;
	while (aux_env != NULL)
	{
		if (search_var(name_var, aux_env->data))
		{
			content_var = cat_var(aux_env->data, ft_strlen(name_var));
			return (content_var);
		}
		else
			aux_env = aux_env->next;
	}
	return (NULL);
}

char	*ft_strjoin_free1(char *s1, char const *s2, size_t size2)
{
	unsigned int	size;
	unsigned int	size1;
	char			*new;

	if (!s1 || !s2)
		return (NULL);
	size1 = ft_strlen(s1);
	size = size1 + size2 + 1;
	new = (char *)ft_calloc(sizeof(char), size);
	if (new == NULL)
		return (NULL);
	ft_strlcpy(new, s1, size1 + 1);
	ft_strlcpy(new + size1, s2, size2 + 1);
	ft_strdel(&s1);
	return (new);
}
