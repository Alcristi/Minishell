/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:28:38 by alcristi          #+#    #+#             */
/*   Updated: 2022/07/20 14:47:14 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strchr_token(char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	if (c == '\0')
		return ((char *) s + ft_strlen(s));
	while (s[i])
	{
		if (s[i] == c)
		{
			if (s[i + 1] != '\0')
			{
				str = ft_strdup(&s[i + 1]);
				s[i + 1] = '\0';
			}
			else
			{
				str = ft_strdup(&s[i]);
				s[i] = '\0';
			}
			return (str);
		}
		i++;
	}
	return (0);
}

void	classify_string(t_token *token, int size)
{
	t_token	*aux;

	aux = token->previus;
	if (aux->is_cmd || aux->is_arg)
		token->is_arg = TRUE;
	else if (aux->is_input && aux->str[0] == '<')
		token->is_input = TRUE;
	else if (aux->is_output && aux->str[0] == '>')
		token->is_output = TRUE;
	else if (aux->is_heredoc && aux->str[0] == '<')
		token->is_heredoc = TRUE;
	else if (aux->is_output_append && aux->str[0] == '>')
		token->is_output_append = TRUE;
	else if (aux == NULL || !aux->is_arg || !aux->is_cmd)
		token->is_cmd = TRUE;
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

void	check_token(t_token *token)
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
		check_token(token);
	if (data)
		free(data);
}

void	convert_for_linked_list(char **str, t_token **tokens)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (i == 0)
			tokens[0] = new_token(str[i]);
		else
			add_node_last_token(tokens, str[i]);
	}
}

void	check_redirect(t_token *tokens)
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

t_token	*parse_cmd(t_token *tokens)
{
	char	**tmp;
	t_token	*aux;

	tmp = ft_split(g_core_var->buff, ' ');
	convert_for_linked_list(tmp, &tokens);
	aux = tokens;
	while (1)
	{
		check_token(tokens);
		if (tokens->next == NULL)
			break ;
		tokens = tokens->next;
	}
	tokens = aux;
	check_redirect(tokens);
	while (1)
	{
		classify_token(tokens);
		if (tokens->next == NULL)
			break ;
		tokens = tokens->next;
	}
	free_double(tmp);
	return (aux);
}
