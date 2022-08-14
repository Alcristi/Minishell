/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 11:56:47 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/14 10:10:59 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_input(t_token *cursor)
{
	if (!ft_strncmp("<", cursor->str, ft_strlen("<"))
		&& (cursor->next == NULL || cursor->next->str[0] == '<'
			|| cursor->next->str[0] == '>' || cursor->next->str[0] == '|'))
	{
		if (cursor->next == NULL)
			printf("bash: syntax error near unexpected token `newline'\n");
		else
			printf("bash: syntax error near unexpected token `%s'\n",
				cursor->next->str);
		return (0);
	}
	return (1);
}

int	is_valid_out(t_token *cursor)
{
	if (!ft_strncmp(">", cursor->str, ft_strlen(">"))
		&& (cursor->next == NULL || cursor->next->str[0] == '<'
			|| cursor->next->str[0] == '>' || cursor->next->str[0] == '|'))
	{
		if (cursor->next == NULL)
			printf("bash: syntax error near unexpected token `newline'\n");
		else
			printf("bash: syntax error near unexpected token `%s'\n",
				cursor->next->str);
		return (0);
	}
	return (1);
}

int	is_valid_heredoc(t_token *cursor)
{
	if (!ft_strncmp("<<", cursor->str, ft_strlen("<<"))
		&& (cursor->next == NULL || cursor->next->str[0] == '<'
			|| cursor->next->str[0] == '>' || cursor->next->str[0] == '|'))
	{
		if (cursor->next == NULL)
			printf("bash: syntax error near unexpected token `newline'\n");
		else
			printf("bash: syntax error near unexpected token `%s'\n",
				cursor->next->str);
		return (0);
	}
	return (1);
}

int	is_valid_out_append(t_token *cursor)
{
	if (!ft_strncmp(">>", cursor->str, ft_strlen(">>"))
		&& (cursor->next == NULL || cursor->next->str[0] == '<'
			|| cursor->next->str[0] == '>' || cursor->next->str[0] == '|'))
	{
		if (cursor->next == NULL)
			printf("bash: syntax error near unexpected token `newline'\n");
		else
			printf("bash: syntax error near unexpected token `%s'\n",
				cursor->next->str);
		return (0);
	}
	return (1);
}

int	is_valid_pipe(t_token *cursor)
{
	if (!ft_strncmp("|", cursor->str, ft_strlen("|"))
		&& (cursor->next == NULL || cursor->previus == NULL
			|| cursor->next->str[0] == '<'
			|| cursor->next->str[0] == '>' || cursor->next->str[0] == '|'))
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return (0);
	}
	return (1);
}
