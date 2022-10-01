/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 19:13:28 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/01 16:35:57 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*resolve_sign(char *str, int size)
{
	char	*result;
	int		sign;
	int		count;

	count = 0;
	sign = 0;
	result = NULL;
	while (count < size && str != NULL)
	{
		if (str[count] == '\'')
			sign = 1;
		else if (str[count] == '\"')
			sign = 2;
		else if (str[count] == '$')
			sign = 3;
		count++;
	}
	if (sign == 1)
		result = resolve_single_quotes(str);
	else if (sign == 2)
		result = resolve_double_quotes(str);
	else if (sign == 3)
		result = resolve_dollar(str);
	return (result);
}

static char	*join_list(t_double_list *list)
{
	t_double_list	*aux;
	char			*str;

	aux = list;
	while (aux->previus != NULL)
		aux = aux->previus;
	str = ft_strdup("");
	while (aux != NULL)
	{
		if (aux->data != NULL)
			str = ft_strjoin_free1(str, aux->data, ft_strlen(aux->data));
		aux = aux->next;
	}
	return (str);
}

int	resolve_string(t_token **tokens)
{
	t_double_list	*list;
	t_double_list	*aux;
	char			*str;

	list = new("");
	cut_quotes(tokens[0]->str, &list);
	aux = list;
	while (aux != NULL)
	{
		str = NULL;
		str = resolve_sign(aux->data, ft_strlen(aux->data));
		if (str != NULL)
		{
			ft_strdel(&aux->data);
			aux->data = ft_strdup(str);
			ft_strdel(&str);
		}
		aux = aux->next;
	}
	str = join_list(list);
	ft_strdel(&tokens[0]->str);
	tokens[0]->str = ft_strdup(str);
	ft_strdel(&str);
	free_list(list);
	return (0);
}
