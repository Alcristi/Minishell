/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 11:55:30 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/04 21:42:17 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	parse_tkn(t_token *tokens)
{
	t_token	*cursor;
	int		is_valid;

	is_valid = 1;
	cursor = tokens;
	while (cursor)
	{
		if (cursor->is_input)
			is_valid = is_valid_input(cursor);
		else if (cursor->is_output)
			is_valid = is_valid_out(cursor);
		else if (cursor->is_heredoc)
			is_valid = is_valid_heredoc(cursor);
		else if (cursor->is_out_append)
			is_valid = is_valid_out_append(cursor);
		else if (cursor->is_pipe)
			is_valid = is_valid_pipe(cursor);
		if (is_valid)
			cursor = cursor->next;
		else
			return (0);
	}
	if (tokens)
		return (1);
	return (0);
}
