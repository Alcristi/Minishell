/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 19:13:28 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/30 13:12:05 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	zerar(char **str, int size)
{
	int	count;

	count = 0;
	while (count < size)
	{
		str[0][count] = '\0';
		count++;
	}
}

static int	next_element(char **temp, t_double_list **aux, int nb)
{
	if (temp[0][0] != '\0')
	{
		add_node_last(&aux[0], temp[0]);
		zerar(temp, ft_strlen(temp[0]));
	}
	return (nb);
}

static char	*init_vars(char *quotes, size_t *c_temp, size_t *index, size_t size)
{
	char	*temp;

	*quotes = 'n';
	*c_temp = 0;
	*index = 0;
	temp = ft_calloc(sizeof(char *), size + 1);
	return (temp);
}

char normasdddd(char *temp,t_double_list **aux,char *str, int	*ctemp)
{
	*c_temp = next_element(&temp, &aux[0], 0);
	temp[0] = str[index];
	return ('\'');
}

static void	cut_quotes(char *str, t_double_list **aux)
{
	char			*temp;
	size_t			index;
	size_t			c_temp;
	char			quotes;

	temp = init_vars(&quotes, &c_temp, &index, ft_strlen(str) + 1);
	while (index < ft_strlen(str))
	{
		if (str[index] == '\'' && quotes == '\'')
		{
			quotes = 'n';
			temp[c_temp] = str[index];
			c_temp = next_element(&temp, &aux[0], -1);
		}
		else if (str[index] == '\'' && quotes == 'n')
		{
			quotes = '\'';
			c_temp = next_element(&temp, &aux[0], 0);
			temp[0] = str[index];
			normasdddd(temp,aux,str, &ctemp);
		}
		else if (str[index] == '\"' && quotes == '\"')
		{
			quotes = 'n';
			temp[c_temp] = str[index];
			c_temp = next_element(&temp, &aux[0], -1);
		}
		else if (str[index] == '\"' && quotes == 'n')
		{
			quotes = '\"';
			c_temp = next_element(&temp, &aux[0], 0);
			temp[0] = str[index];
		}
		else
			temp[c_temp] = str[index];
		c_temp++;
		index++;
	}
	if (c_temp != 0)
		add_node_last(&aux[0], temp);
	ft_strdel(&temp);
}

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
