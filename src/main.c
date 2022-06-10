/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 11:55:21 by alcristi          #+#    #+#             */
/*   Updated: 2022/06/10 20:09:12 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define TRUE 1
#define FALSE 0
t_core_var *g_core_var;

typedef struct s_token
{
	char *str;
	int is_pipe;
	int is_input;
	int is_output;
	int is_heredoc;
	int is_output_append;
	int is_cmd;
	int is_arg;
	int single_quotes;
	int double_quotes;
	struct s_token *next;
	struct s_token *previus;
} t_token;

t_token *new_token(char *data)
{
	t_token *list;

	list = malloc(sizeof(t_token));
	list->next = NULL;
	list->previus = NULL;
	list->str = ft_strdup(data);
	list->is_arg = FALSE;
	list->is_cmd = FALSE;
	list->is_heredoc = FALSE;
	list->is_input = FALSE;
	list->is_output = FALSE;
	list->is_output_append = FALSE;
	list->is_pipe = FALSE;
	list->single_quotes = FALSE;
	list->double_quotes = FALSE;
	return (list);
}

void add_node_last_token(t_token **node, char *data)
{
	t_token *new_node;
	t_token *aux;

	aux = *node;
	while (node[0]->next)
		*node = node[0]->next;
	new_node = new_token(data);
	new_node->previus = *node;
	node[0]->next = new_node;
	*node = aux;
}

void add_node_middle_token(t_token **node,char *data)
{
	t_token *new_node;

	new_node = new_token(data);
	new_node->next = node[0]->next;
	new_node->previus = *node;
	node[0]-> next = new_node;
}

char *clear_quotes(char *str)
{
	char **tmp;
	int i;
	char *aux;
	char *text;
	i = 0;
	tmp = ft_split(str, '\"');
	while (tmp[i])
	{
		if (i == 0)
			text = ft_strjoin("", tmp[i]);
		else
		{
			aux = text;
			text = ft_strjoin(aux, tmp[i]);
			free(aux);
		}
		i++;
	}
	return text;
}

int ft_get_dir(char *str)
{
	int size;

	size = ft_strlen(str) - 1;
	while (str[size] != '/')
		size--;
	return (size + 1);
}

void init_dir(t_prompt *prompt)
{
	prompt->dir.path_current = getcwd(NULL, 0);
	prompt->dir.size_path = ft_strlen(g_core_var->prompt.dir.path_current);
	prompt->dir.position = ft_get_dir(g_core_var->prompt.dir.path_current);
	prompt->dir.current = ft_substr(g_core_var->prompt.dir.path_current,
									g_core_var->prompt.dir.position, g_core_var->prompt.dir.size_path);
}

void free_dir(t_core_var *core)
{
	free(core->prompt.dir.path_current);
	free(core->prompt.dir.current);
}

void str_prompt(void)
{
	char *aux;

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

void free_core(void)
{
	free(g_core_var->prompt.prompt);
	free(g_core_var->buff);
	free_list(g_core_var->env);
	free(g_core_var);
}

// typedef struct  s_token
// {
// 	char *value;
// 	char *type;
// }t_token;

char	*ft_strchr_token(char *s, int c)
{
	int	i;

	char *str;
	i = 0;
	if (c == '\0')
		return ((char *) s + ft_strlen(s));
	if (c > 256)
		c -= 256;
	while (s[i])
	{
		if (s[i] == c)
		{
			//s[i] = '\0';
			str = ft_strdup(&s[i + 1]);
			s[i + 1] = '\0';
			return (str);
		}
		i++;
	}
	return (0);
}

void classify_token(t_token *token)
{
	int size;


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
		else if (token->previus->is_cmd == TRUE || token->previus->is_arg == TRUE)
			token->is_arg = TRUE;
	}
}

void check_token(t_token *token)
{
	int	size;
	char *data;
	size = ft_strlen(token->str);
	if(ft_strchr(token->str,'<') && size != 1)
	{
		data = ft_strchr_token(token->str,'<');
		add_node_middle_token(&token,data);
	}
}

t_token *parse_cmd(t_token *tokens)
{
	char **tmp;
	int i;
	t_token *aux;

	tmp = ft_split(g_core_var->buff, ' ');
	i = 0;
	while (tmp[i])
	{

		if (i == 0)
			tokens = new_token(tmp[i]);
		else
			add_node_last_token(&tokens, tmp[i]);
		i++;
	}
	aux = tokens;
	while (1)
	{
		check_token(tokens);
		if (tokens->next == NULL)
			break;
		tokens = tokens->next;
	}
	tokens = aux;
	while (1)
	{
		classify_token(tokens);
		if (tokens->next == NULL)
			break;
		tokens = tokens->next;
	}
	return (tokens);
}

void prompt(void)
{
	t_token *tokens;

	while (1)
	{
		str_prompt();
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		if (g_core_var->buff == NULL)
		{
			printf("exit\n");
			break;
		}
		if (!ft_strncmp(g_core_var->buff, "exit", ft_strlen("exit")))
			break;
		else if (!ft_strncmp(g_core_var->buff, "print", ft_strlen("print")))
			print_linked_list(g_core_var->env);
		else
		{
			tokens = parse_cmd(tokens);
		}
		add_history(g_core_var->buff);
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
	}
}

void init_core(char **env)
{
	int i;

	i = 0;
	g_core_var = (t_core_var *)malloc(sizeof(t_core_var));
	g_core_var->prompt.user = getenv("USER");
	while (env[i])
	{
		if (i == 0)
			g_core_var->env = new (env[i]);
		else
			add_node_last(&g_core_var->env, env[i]);
		i++;
	}
}

int main(int argc, char *argv[], char *env[])
{
	int i;

	i = 0;
	init_core(env);
	prompt();
	rl_clear_history();
	free_core();
	return (0);
}
