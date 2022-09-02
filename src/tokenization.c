/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:28:38 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/03 00:58:04 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	convert_for_linked_list(char **str, t_token **tokens)
{
	int	count;
	int	index;

	index = -1;
	while (str[++index])
	{
		count = 0;
		while (str[index][count])
		{
			if (str[index][count] == 7)
				str[index][count] = ' ';
			count++;
		}
		if (index == 0)
			tokens[0] = new_token(str[index]);
		else
			add_node_last_token(tokens, str[index]);
	}
}

static void	convert_space_buff(void)
{
	int	size;
	int	count;
	int	active_s_quotes;
	int	active_d_quotes;

	count = 0;
	active_s_quotes = 0;
	active_d_quotes = 0;
	size = ft_strlen(g_core_var->buff);
	while (count < size)
	{
		if (g_core_var->buff[count] == '\'' && active_s_quotes == 0)
			active_s_quotes = 1;
		else if (g_core_var->buff[count] == '\"' && active_d_quotes == 0)
			active_s_quotes = 1;
		else if (g_core_var->buff[count] == '\'' && active_s_quotes == 1)
			active_s_quotes = 0;
		else if (g_core_var->buff[count] == '\"' && active_d_quotes == 1)
			active_s_quotes = 0;
		else if (g_core_var->buff[count] == ' '
			&& (active_s_quotes || active_d_quotes))
			g_core_var->buff[count] = 7;
		count++;
	}
}

static t_token	*build_tokens(void)
{
	char	**tmp;
	t_token	*tokens;

	tokens = NULL;
	convert_space_buff();
	tmp = ft_split(g_core_var->buff, ' ');
	convert_for_linked_list(tmp, &tokens);
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
