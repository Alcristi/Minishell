/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:22:16 by alcristi          #+#    #+#             */
/*   Updated: 2022/10/03 22:29:40 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_exit(char *buff)
{
	if (buff == NULL)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	return (0);
}

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
	g_core_var->prompt.prompt = ft_strjoin(aux, g_core_var->prompt.dir.path);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, END);
	free(aux);
	aux = g_core_var->prompt.prompt;
	g_core_var->prompt.prompt = ft_strjoin(aux, "\e[1m$\e[0m ");
	free_dir(g_core_var);
	free(aux);
}

static void	check_execute(t_token **tokens, t_stacks **stacks)
{
	tokens[0] = tokenization_cmd(tokens[0]);
	g_core_var->exit_code = 0;
	g_core_var->fd_in = 0;
	g_core_var->fd_out = 0;
	if (parse_tkn(tokens[0]))
	{
		stacks[0] = build_stack(tokens[0]);
		execute(stacks, tokens);
		free_stacks(&stacks[0]);
	}
}

char	**convert_env_for_string(void)
{
	t_double_list	*cursor;
	int				count;
	char			**envp;

	count = 0;
	cursor = g_core_var->env;
	while (cursor)
	{
		cursor = cursor->next;
		count++;
	}
	cursor = g_core_var->env;
	envp = ft_calloc(sizeof(char *), count + 1);
	count = 0;
	while (cursor)
	{
		envp[count] = strdup(cursor->data);
		cursor = cursor->next;
		count++;
	}
	envp[count] = NULL;
	return (envp);
}

void	prompt(void)
{
	t_token		*tokens;
	t_stacks	*stacks;
	char		quotes;

	while (1)
	{
		str_prompt();
		signal(SIGINT, sig_handle);
		signal(SIGQUIT, SIG_IGN);
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		add_history(g_core_var->buff);
		if (check_exit(g_core_var->buff))
			break ;
		quotes = convert_space_buff();
		if (quotes != 'n')
			printf("minishell: error, the (%s) must be closed\n", quotes);
		if (ft_strlen(g_core_var->buff) > 0 && quotes == 'n')
			check_execute(&tokens, &stacks);
		if (ft_strlen(g_core_var->buff) > 0)
			free_token(&tokens);
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
	}
}
