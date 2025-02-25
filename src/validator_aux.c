/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:51:01 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/04 18:07:18 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_cod(t_stacks *stacks, t_token *tokens, int cod_error)
{
	while (stacks->stack_cmd->previus)
		stacks->stack_cmd = stacks->stack_cmd->previus;
	free_exec(&stacks, &tokens);
	exit (cod_error);
}

int	check_cmd(char *str, t_stacks *stacks, t_token *tokens)
{
	struct stat	info_stat;

	if (stat(str, &info_stat) != -1 && S_ISDIR(info_stat.st_mode))
	{
		error_message(str, ": Is a directory\n");
		exit_cod(stacks, tokens, IS_DIRECTORY);
	}
	if (!access(str, F_OK))
	{
		if (access(str, X_OK))
		{
			error_message(str, ": Permission denied\n");
			exit_cod(stacks, tokens, PERMISSION_DENIED);
		}
	}
	else
	{
		error_message(str, ": No such file or directory\n");
		exit_cod(stacks, tokens, CMD_NOT_FOUND);
	}
	return (1);
}

int	size_list_string(char **str)
{
	int	count;

	count = 0;
	while (str[count])
		count ++;
	return (count);
}

int	exist_cmd(char *str, t_stacks **stacks, t_token **tokens)
{
	char	*tmp;
	char	**aux;
	int		size_list;

	if (access(str, F_OK))
	{
		while (stacks[0]->stack_cmd->previus)
			stacks[0]->stack_cmd = stacks[0]->stack_cmd->previus;
		aux = ft_split(str, '/');
		size_list = size_list_string(aux);
		tmp = ft_strdup(aux[size_list - 1]);
		free_exec(stacks, tokens);
		free_double(aux);
		system_error(CMD_NOT_FOUND, tmp);
	}
	return (1);
}

int	is_valid(t_token *cmd, t_stacks **stacks, t_token **tokens)
{
	char			**tmp_path;
	int				count;
	char			*tmp;

	count = 1;
	if (ft_strchr(cmd->str, '/'))
		return (check_cmd(cmd->str, stacks[0], tokens[0]));
	else
	{
		tmp_path = load_path(cmd->str, stacks, tokens);
		tmp = ft_strjoin("/", cmd->str);
		free(cmd->str);
		cmd->str = ft_strjoin(tmp_path[0], tmp);
		while (access(cmd->str, F_OK) && tmp_path[count])
		{
			free(cmd->str);
			cmd->str = ft_strjoin(tmp_path[count], tmp);
			count++;
		}
		free(tmp);
		free_double(tmp_path);
		if (exist_cmd(cmd->str, stacks, tokens))
			return (TRUE);
	}
	return (TRUE);
}
