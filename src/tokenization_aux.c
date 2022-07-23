/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 14:49:04 by esilva-s          #+#    #+#             */
/*   Updated: 2022/07/22 20:53:15 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	classify_string(t_token *token, int size)
{
	t_token	*aux;

	aux = token->previus;
	if (aux == NULL || !aux->is_arg || !aux->is_cmd)
		token->is_cmd = TRUE;
	else if (aux->is_cmd || aux->is_arg)
		token->is_arg = TRUE;
	else if (aux->is_input && aux->str[0] == '<')
		token->is_input = TRUE;
	else if (aux->is_output && aux->str[0] == '>')
		token->is_output = TRUE;
	else if (aux->is_heredoc && aux->str[0] == '<')
		token->is_heredoc = TRUE;
	else if (aux->is_output_append && aux->str[0] == '>')
		token->is_output_append = TRUE;
}

void	classify_token(t_token *token)
{
	int	size;

	size = ft_strlen(token->str);
	if (!ft_strncmp(token->str, "<<", size) && size == 2)
		token->is_heredoc = TRUE;
	else if (!ft_strncmp(token->str, ">>", size) && size == 2)
		token->is_output_append = TRUE;
	else if (!ft_strncmp(token->str, "<", size))
		token->is_input = TRUE;
	else if (!ft_strncmp(token->str, ">", size))
		token->is_output = TRUE;
	else if (!ft_strncmp(token->str, "|", size))
		token->is_pipe = TRUE;
	else
		classify_string(token, size);
}

void	normalize_token(t_token *token)
{
	int		size;
	char	*data;

	data = NULL;
	size = ft_strlen(token->str);
	if (ft_strnstr(token->str, ">>", size) && size != 2)
		data = ft_strnstr_token(token->str, ">>", size);
	else if (ft_strnstr(token->str, "<<", size) && size != 2)
		data = ft_strnstr_token(token->str, "<<", size);
	else if (ft_strchr(token->str, '<') && size != 1)
		data = ft_strchr_token(token->str, '<');
	else if (ft_strchr(token->str, '>') && size != 1)
		data = ft_strchr_token(token->str, '>');
	else if (ft_strchr(token->str, '|') && size != 1)
		data = ft_strchr_token(token->str, '|');
	if (data)
		add_node_middle_token(&token, data);
	if (((ft_strchr(token->str, '|') || ft_strchr(token->str, '>')
				|| ft_strchr(token->str, '<')) && size != 1)
		|| ((ft_strnstr(token->str, "<<", size)
				|| ft_strnstr(token->str, ">>", size)) && size != 2))
		normalize_token(token);
	if (data)
		free(data);
}

void	normalize_redirect(t_token *tokens)
{
	t_token	*cursor;
	t_token	*tmp;
	char	aux;

	cursor = tokens;
	while (cursor->next)
	{
		if ((cursor->str[0] == '<' && cursor->next->str[0] == '<')
			|| (cursor->str[0] == '>' && cursor->next->str[0] == '>'))
		{
			aux = cursor->str[0];
			free(cursor->str);
			if (aux == '<')
				cursor->str = ft_strdup("<<");
			else
				cursor->str = ft_strdup(">>");
			tmp = cursor->next;
			cursor->next = tmp->next;
			cursor->next->previus = cursor;
			free(tmp->str);
			free(tmp);
		}
		cursor = cursor->next;
	}
}
