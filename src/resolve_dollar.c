/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:37:06 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/05 23:09:16 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	catch_pos_dollar(char *str)
{
	int	count;
	int	size;

	count = 0;
	size = ft_strlen(str);
	while (count < size - 1)
	{
		if (str[count] == '$' && valid_name_char(str[count + 1]))
			return (count);
		count++;
	}
	return (-1);
}

static char	*cat_name_var(char *str, int pos)
{
	size_t		count;
	char		*name_var;

	count = 0;
	if (str[pos + 1] == '?')
		return (ft_strdup("?"));
	name_var = ft_calloc(sizeof(char *), ft_strlen(str));
	while (count < ft_strlen(str))
	{
		pos++;
		if (valid_name_char(str[pos]))
			name_var[count] = str[pos];
		else
			break ;
		count++;
	}
	name_var[count] = '\0';
	if (count == 0)
	{
		free(name_var);
		return (NULL);
	}
	else
		return (name_var);
}

static char	*subst_dollar(char *str, char *var, int pos, int len_name)
{
	char	*result;
	char	*aux;
	int		end_pos;
	int		len;

	result = NULL;
	if (var == NULL)
		aux = strdup("");
	else
		aux = strdup(var);
	if (pos != 0)
	{
		result = ft_calloc(sizeof(char *), pos + ft_strlen(aux) + 1);
		ft_strlcpy(result, str, pos + 1);
		result = ft_strjoin_free1(result, aux, ft_strlen(aux));
	}
	else
		result = ft_strdup(aux);
	end_pos = pos + 1 + len_name;
	len = ft_strlen(str) - end_pos;
	if (len > 0)
		result = ft_strjoin_free1(result, str + end_pos, len);
	ft_strdel(&aux);
	return (result);
}

char	*resolve_dollar(char *str)
{
	char	*result;
	char	*var;
	char	*name_var;
	int		pos;

	pos = catch_pos_dollar(str);
	if (pos < 0)
		return (NULL);
	name_var = cat_name_var(str, pos);
	if (name_var == NULL)
		return (NULL);
	if (!ft_strncmp(name_var, "?", 1))
		var = expansion_exit_code();
	else
		var = catch_var(name_var);
	result = subst_dollar(str, var, pos, ft_strlen(name_var));
	ft_strdel(&name_var);
	ft_strdel(&var);
	return (result);
}
