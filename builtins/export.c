/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 19:48:41 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/05 23:29:22 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static size_t	count_letters(char const *s, char c)
{
	int	amount_of_letters;

	amount_of_letters = 0;
	while (s[amount_of_letters] != c && s[amount_of_letters])
		amount_of_letters++;
	return (amount_of_letters);
}

char	**ft_split_first(char const *s, char c)
{
	char	**list;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	list = malloc(sizeof(char *) * (2 + 1));
	if (!list)
		return (0);
	while (i < 2 && *s)
	{
		if (*s != c)
		{
			if (i == 0)
			{
				list[i] = ft_substr(s, 0, count_letters(s, c));
				if (!list[i++])
					return (0);
				s += count_letters(s, c);
			}
			else
			{
				list[i] = ft_substr(s, 0, count_letters(s, '\a'));
				if (!list[i++])
					return (0);
				s += count_letters(s, c);
			}
		}
		else
			s++;
	}
	list[2] = NULL;
	return (list);
}

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
	char			*name;
	size_t			pos_equal;
	size_t			count;

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

static void	print_vars(void)
{
	t_double_list	*aux;
	char			**str;

	aux = g_core_var->env;
	while (aux->previus != NULL)
		aux = aux->previus;
	while (aux != NULL)
	{
		str = ft_split_first(aux->data, '=');
		printf("declare -x %s=\"%s\"\n", str[0], str[1]);
		free_double(str);
		str = NULL;
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

int	check_names(char **str)
{
	int		count;
	char	*error;

	count = 0;
	while (str[count])
	{
		if (((!ft_strchr(str[count], '=') && count == 1)
				|| (ft_strchr(str[count], '=') && count != 1))
			&& verify_name(str[count]))
		{
			error = ft_strjoin("`", str[count]);
			error = ft_strjoin_gnl(error, "': not a valid identifier");
			ft_putstr_fd(error, 2);
			free(error);
			return (1);
		}
		count++;
	}
	return (0);
}

int	bt_export(char **arg)
{
	char	*name;
	char	*error;

	if (arg[1] == NULL)
	{
		print_vars();
		return (0);
	}
	if (check_names(arg))
		return (1);
	if (!ft_strchr(arg[1], '='))
		return (0);
	name = catch_name(arg[1]);
	if (name == NULL || verify_name(name))
	{
		if (name)
			free(name);
		error = ft_strjoin("`", arg[1]);
		error = ft_strjoin_gnl(error, "': not a valid identifier");
		ft_putstr_fd(error, 2);
		free(error);
		return (1);
	}
	if (localize_var(name, arg[1]))
	{
		free(name);
		return (0);
	}
	add_node_last(&g_core_var->env, arg[1]);
	free(name);
	return (0);
}
