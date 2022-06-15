/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:28:38 by alcristi          #+#    #+#             */
/*   Updated: 2022/06/15 19:41:33 by alcristi         ###   ########.fr       */
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

void	classify_token(t_token *token)
{
	int	size;

	size = ft_strlen(token->str);
	if (!ft_strncmp(token->str, "<<", size))
		token->is_heredoc = TRUE;
	else if (!ft_strncmp(token->str, ">>", size))
		token->is_output_append = TRUE;
	else if (!ft_strncmp(token->str, "<", size))
		token->is_input = TRUE;
	else if (!ft_strncmp(token->str, ">", size))
		token->is_output = TRUE;
	else if (!ft_strncmp(token->str, "|", size))
		token->is_pipe = TRUE;
	else
	{
		if (token->previus == NULL || token->previus->is_cmd == FALSE)
			token->is_cmd = TRUE;
		else if (token->previus->is_cmd == TRUE
			|| token->previus->is_arg == TRUE)
			token->is_arg = TRUE;
	}
}

void	check_token(t_token *token)
{
	int		size;
	char	*data;

	data = NULL;
	size = ft_strlen(token->str);
	if (ft_strchr(token->str, '<') && size != 1)
	{
		data = ft_strchr_token(token->str, '<');
		add_node_middle_token(&token, data);
	}
	else if (ft_strchr(token->str, '>') && size != 1)
	{
		data = ft_strchr_token(token->str, '>');
		add_node_middle_token(&token, data);
	}
	else if (ft_strchr(token->str, '|') && size != 1)
	{
		data = ft_strchr_token(token->str, '|');
		add_node_middle_token(&token, data);
	}
	if ((ft_strchr(token->str, '|') || ft_strchr(token->str, '>')
			|| ft_strchr(token->str, '<')) && size != 1)
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
