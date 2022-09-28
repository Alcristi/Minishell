/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_dollarr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:37:06 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/28 11:04:36 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	valid_name_char(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)
		|| (c >= 48 && c <= 57) || c == 95)
		return (1);
	return (0);
}

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
	int		count;
	char	*name_var;

	count = 0;
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
	int		end_pos;
	int		len;

	result = NULL;
	if (pos != 0)
	{
		result = ft_calloc(sizeof(char *), pos + ft_strlen(var) + 1);
		ft_strlcpy(result, str, pos + 1);
		result = ft_strjoin_free1(result, var, ft_strlen(var));
	}
	else
		result = ft_strdup(var);
	end_pos = pos + 1 + len_name;
	len = ft_strlen(str) - end_pos;
	if (len > 0)
		result = ft_strjoin_free1(result, str + end_pos, len);
	return (result);
}

char	*resolve_dollar(char *str)
{
	char	*result;
	char	*var;
	char	*name_var;
	int		pos;

	if (!ft_strncmp(str,"$?",2))
	{
		if (g_core_var->exit_code == 131)
			return (ft_itoa(g_core_var->exit_code));
		else
			return (ft_itoa(WEXITSTATUS(g_core_var->exit_code)));
	}
	pos = catch_pos_dollar(str);
	if (pos < 0)
		return (NULL);
	name_var = cat_name_var(str, pos);
	if (name_var == NULL)
		return (NULL);
	var = catch_var(name_var);
	if (var == NULL)
	{
		free(name_var);
		return (NULL);
	}
	result = subst_dollar(str, var, pos, ft_strlen(name_var));
	free(name_var);
	free(var);
	return (result);
}
