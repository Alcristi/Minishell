/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 14:49:04 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/29 18:00:24 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_cmd(t_token *token)
{
	if ((token->is_input || token->is_heredoc) && token->str[0] == '<')
		return (0);
	else if ((token->is_output || token->is_out_append) && token->str[0] == '>')
		return (0);
	return (1);
}

static void	classify_string(t_token *token, int size)
{
	t_token	*aux;

	aux = token->previus;
	if (aux == NULL || (!aux->is_arg && !aux->is_cmd && is_cmd(aux)))
		token->is_cmd = TRUE;
	else if (aux->is_cmd || aux->is_arg)
		token->is_arg = TRUE;
	else if (aux->is_input && aux->str[0] == '<')
		token->is_input = TRUE;
	else if (aux->is_output && aux->str[0] == '>')
		token->is_output = TRUE;
	else if (aux->is_heredoc && aux->str[0] == '<')
		token->is_heredoc = TRUE;
	else if (aux->is_out_append && aux->str[0] == '>')
		token->is_out_append = TRUE;
}

void	classify_token(t_token *token)
{
	int	size;

	size = ft_strlen(token->str);
	if (!ft_strncmp(token->str, "<<", size) && size == 2 && !token->quotes)
		token->is_heredoc = TRUE;
	else if (!ft_strncmp(token->str, ">>", size) && size == 2 && !token->quotes)
		token->is_out_append = TRUE;
	else if (!ft_strncmp(token->str, "<", size) && !token->quotes)
		token->is_input = TRUE;
	else if (!ft_strncmp(token->str, ">", size) && !token->quotes)
		token->is_output = TRUE;
	else if (!ft_strncmp(token->str, "|", size) && !token->quotes)
		token->is_pipe = TRUE;
	else
		classify_string(token, size);
}

int	is_normalize(t_token *token, int size)
{
	if (!token->quotes && (((ft_strchr(token->str, '|')
					|| (ft_strchr(token->str, '>')
						&& ft_strncmp(token->str, ">>", ft_strlen(token->str)))
					|| (ft_strchr(token->str, '<')
						&& ft_strncmp(token->str, "<<", ft_strlen(token->str))))
				&& size != 1)
			|| ((ft_strnstr(token->str, "<<", size)
					|| ft_strnstr(token->str, ">>", size)) && size != 2)))
	{
		return (0);
	}
	return (1);
}

void	normalize_token(t_token *token)
{
	int		size;
	char	*data;

	data = NULL;
	size = ft_strlen(token->str);
	if (ft_strnstr(token->str, ">>", size) && size != 2 && !token->quotes)
		data = ft_strnstr_token(token->str, ">>", size);
	else if (ft_strnstr(token->str, "<<", size) && size != 2 && !token->quotes)
		data = ft_strnstr_token(token->str, "<<", size);
	else if (ft_strchr(token->str, '<') && size != 1 && !token->quotes
		&& ft_strncmp(token->str, "<<", ft_strlen(token->str)))
		data = ft_strchr_token(token->str, '<');
	else if (ft_strchr(token->str, '>') && size != 1 && !token->quotes
		&& ft_strncmp(token->str, ">>", ft_strlen(token->str)))
		data = ft_strchr_token(token->str, '>');
	else if (ft_strchr(token->str, '|') && size != 1 && !token->quotes)
		data = ft_strchr_token(token->str, '|');
	if (data)
		add_node_middle_token(&token, data);
	if (!is_normalize(token, size))
		normalize_token(token);
	if (data)
		free(data);
}
