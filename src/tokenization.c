/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:28:38 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/30 10:54:31 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	convert_for_linked_list(char **str, t_token **tokens)
{
	int	count;
	int	index;
	int	size;

	index = -1;
	while (str[++index])
	{
		count = 0;
		while (str[index][count])
		{
			if (str[index][count] == 7)
				str[index][count] = 32;
			count++;
		}
		size = ft_strlen(str[index]);
		if (index == 0 && ft_strncmp(str[index], "''", size)
			&& ft_strncmp(str[index], "\"\"", size))
			tokens[0] = new_token(str[index]);
		else if (ft_strncmp(str[index], "''", size)
			&& ft_strncmp(str[index], "\"\"", size))
			add_node_last_token(tokens, str[index]);
	}
}

static t_token	*build_tokens(void)
{
	char	**tmp;
	t_token	*tokens;
	t_token	*aux;

	tokens = NULL;
	tmp = ft_split(g_core_var->buff, 32);
	convert_for_linked_list(tmp, &tokens);
	aux = tokens;
	while (aux != NULL)
	{
		resolve_string(&aux);
		aux = aux->next;
	}
	free_double(tmp);
	return (tokens);
}

static void	multiplex_fuction(t_token *tokens, int cicle)
{
	if (cicle == 0)
		normalize_token(tokens);
	if (cicle == 1)
		classify_token(tokens);
}

t_token	*tokenization_cmd(t_token *tokens)
{
	t_token	*aux;
	int		cicle;

	cicle = 0;
	tokens = build_tokens();
	aux = tokens;
	if (!tokens)
		return (NULL);
	while (1)
	{
		multiplex_fuction(tokens, cicle);
		if (tokens->next == NULL && cicle == 0)
		{
			tokens = aux;
			cicle = 1;
			continue ;
		}
		else if (tokens->next == NULL && cicle == 1)
			break ;
		tokens = tokens->next;
	}
	return (aux);
}
