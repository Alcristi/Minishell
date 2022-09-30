/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_temp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 03:17:26 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/30 11:25:05 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	search_var(char *var, char *env)
{
	size_t		count;
	size_t		index;

	count = 0;
	index = 0;
	while (env[index])
	{
		if (env[index] == '=')
			break ;
		index++;
	}
	if (index == ft_strlen(env))
		return (0);
	while (env[count] && var[count])
	{
		if (env[count] != var[count])
			return (0);
		count++;
	}
	if (count == index)
		return (1);
	else
		return (0);
}

char	*cat_var(char *env, int len_name_var)
{
	int		size;
	int		count;
	char	*result;

	size = ft_strlen(env) - len_name_var;
	count = 0;
	result = ft_calloc(sizeof(char), size);
	while (env[count + len_name_var + 1])
	{
		result[count] = env[count + len_name_var + 1];
		count++;
	}
	result[count] = '\0';
	return (result);
}

void	free_exec(t_stacks **stacks, t_token **tokens)
{
	free_stacks(stacks);
	free_token(tokens);
	free_core();
	rl_clear_history();
	stacks = NULL;
	tokens = NULL;
}

char	*expansion_exit_code(void)
{
	if (g_core_var->exit_code == 131)
		return (ft_itoa(g_core_var->exit_code));
	else if (g_core_var->exit_code == 2)
		return (ft_itoa(INTERRUPT_SIG_INT));
	else if (g_core_var->exit_code == 1)
		return (ft_itoa(EXIT_FAILURE));
	else
		return (ft_itoa(WEXITSTATUS(g_core_var->exit_code)));
}

char	*mount_message_eof(char *s)
{
	char	*str;

	str = ft_strdup("\nminishell: warning: heredoc ");
	str = ft_strjoin_gnl(str, "delimited by EOF - wanted '");
	str = ft_strjoin_gnl(str, s);
	return (ft_strjoin_gnl(str, "'\n"));
}
