/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 15:22:16 by alcristi          #+#    #+#             */
/*   Updated: 2022/09/02 22:34:37 by esilva-s         ###   ########.fr       */
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

static void	check_execute(t_token **tokens, t_stacks **stacks)
{
	normalize_quotes();
	tokens[0] = tokenization_cmd(tokens[0]);
	if (parse_tkn(tokens[0]))
	{
		stacks[0] = build_stack(tokens[0]);
		execute(stacks[0], tokens[0]);
		free_stacks(&stacks[0]);
	}
}

//função com o loop do prompt
void	prompt(void)
{
	t_token		*tokens;
	t_stacks	*stacks;

	while (1)
	{
		signal(SIGINT, sig_handle);
		str_prompt();
		g_core_var->buff = readline(g_core_var->prompt.prompt);
		add_history(g_core_var->buff);
		if (check_exit(g_core_var->buff))
			break ;
		else if (!check_print(g_core_var->buff, g_core_var->env)
			&& ft_strlen(g_core_var->buff) > 0)
			check_execute(&tokens, &stacks);
		if (ft_strlen(g_core_var->buff) > 0)
			free_token(&tokens);
		free(g_core_var->buff);
		free(g_core_var->prompt.prompt);
	}
}
