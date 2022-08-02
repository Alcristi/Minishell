/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:22:16 by alcristi          #+#    #+#             */
/*   Updated: 2022/08/02 10:21:21 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	str_prompt(void)
{
	char	*aux;

	init_dir(&g_core_var->prompt);
	aux = ft_strjoin(GREEN, g_core_var->prompt.user);
	g_core_var->prompt.prompt = ft_strjoin(aux, ": ");
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, END);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, BLUE);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, g_core_var->prompt.dir.current);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, END);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, "\e[1m$\e[0m ");
	free_dir(g_core_var);
	free(aux);
}

static int	check_exit(char *buff)
{
	if (buff == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	else if (!ft_strncmp(buff, "exit", ft_strlen("exit")))
		return (1);
	return (0);
}

static int	check_print(char *buff, t_double_list *env)
{
	if (!ft_strncmp(buff, "print", ft_strlen("print")))
	{
		print_linked_list(env);
		return (1);
	}
	return (0);
}

int	parse_tkn(t_token *tokens)
{
	t_token	*cursor;

	cursor = tokens;
	while (cursor)
	{
		if(cursor->is_input)
		{
			if(!ft_strncmp("<",cursor->str,ft_strlen("<")) && (cursor->next == NULL || cursor->next->str[0] == '<' || cursor->next->str[0] == '>' ||cursor->next->str[0] == '|'  ))
			{
				if(cursor->next == NULL)
				{
					printf("bash: syntax error near unexpected token `newline'\n");
					return(0);
				}
				else
				{
					printf("bash: syntax error near unexpected token `%s'\n",cursor->next->str);
					return(0);
				}
			}
		}
		else if(cursor->is_output)
		{
			if(!ft_strncmp(">",cursor->str,ft_strlen(">")) && (cursor->next == NULL || cursor->next->str[0] == '<' || cursor->next->str[0] == '>' ||cursor->next->str[0] == '|'  ))
			{
				if(cursor->next == NULL)
				{
					printf("bash: syntax error near unexpected token `newline'\n");
					return(0);
				}
				else
				{
					printf("bash: syntax error near unexpected token `%s'\n",cursor->next->str);
					return(0);
				}
			}
		}
		else if(cursor->is_heredoc)
		{
			if(!ft_strncmp("<<",cursor->str,ft_strlen("<<")) && (cursor->next == NULL || cursor->next->str[0] == '<' || cursor->next->str[0] == '>' ||cursor->next->str[0] == '|'  ))
			{
				if(cursor->next == NULL)
				{
					printf("bash: syntax error near unexpected token `newline'\n");
					return(0);
				}
				else
				{
					printf("bash: syntax error near unexpected token `%s'\n",cursor->next->str);
					return(0);
				}
			}
		}
		else if(cursor->is_out_append)
		{
			if(!ft_strncmp(">>",cursor->str,ft_strlen(">>")) && (cursor->next == NULL || cursor->next->str[0] == '<' || cursor->next->str[0] == '>' ||cursor->next->str[0] == '|'  ))
			{
				if(cursor->next == NULL)
				{
					printf("bash: syntax error near unexpected token `newline'\n");
					return(0);
				}
				else
				{
					printf("bash: syntax error near unexpected token `%s'\n",cursor->next->str);
					return(0);
				}
			}
		}
		else if(cursor->is_pipe)
		{
			if(!ft_strncmp("|",cursor->str,ft_strlen("|")) && (cursor->next == NULL || cursor->previus == NULL|| cursor->next->str[0] == '<' || cursor->next->str[0] == '>' ||cursor->next->str[0] == '|'  ))
			{
					printf("bash: syntax error near unexpected token `|'\n");
					return(0);
			}
		}
		cursor = cursor->next;
	}
	return (1);
}

void	prompt(void)
{
	t_token	*tokens;

	while (1)
	{
		str_prompt();
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		add_history(g_core_var->buff);
		if (check_exit(g_core_var->buff))
			break ;
		else if (!check_print(g_core_var->buff, g_core_var->env))
		{
			normalize_quotes();
			printf("%s\n", g_core_var->buff);
			tokens = tokenization_cmd(tokens);
			parse_tkn(tokens);
		}
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
		free_token(tokens);
	}
}
