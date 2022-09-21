/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esilva-s <esilva-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 23:21:27 by esilva-s          #+#    #+#             */
/*   Updated: 2022/09/21 00:19:42 by esilva-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <fcntl.h>

# include<readline/readline.h>
# include<readline/history.h>

# include "./libft.h"

# define GREEN "\e[1;32m"
# define PURPLE "\e[1;35m"
# define BLUE "\e[1;34m"
# define END "\e[0m"
# define TRUE 1
# define FALSE 0

typedef struct s_token
{
	char			*str;
	int				is_pipe;
	int				is_input;
	int				is_output;
	int				is_heredoc;
	int				is_out_append;
	int				is_cmd;
	int				is_arg;
	int				single_quotes;
	int				double_quotes;
	struct s_token	*next;
	struct s_token	*previus;
}	t_token;

typedef struct s_double_list
{
	char					*data;
	struct s_double_list	*next;
	struct s_double_list	*previus;
}	t_double_list;

typedef struct s_dir
{
	char	*current;
	char	*path_current;
	int		size_path;
	int		position;
}	t_dir;

typedef struct s_prompt
{
	char	*user;
	t_dir	dir;
	char	*prompt;
}	t_prompt;

typedef struct s_stack_aux
{
	t_token	*stack_input;
	t_token	*stack_out;
	t_token	*stack_cmd;
	t_token	*stack_herodoc;
}	t_stacks;

typedef struct s_core_var
{
	char			*buff;
	t_prompt		prompt;
	t_double_list	*env;
	char			**envp;
	int				confirm;
	int				fd_in;
	int				fd_out;
	int				exit_code;
}t_core_var;

extern t_core_var	*g_core_var;

void	free_core(void);

t_double_list	*new(char *data);

int	bt_pwd(void);
int	bt_unset(char *name);
int	bt_export(char *arg);
void	bt_env(void);
int	bt_echo(int nb, char **args);

void	add_node_front(t_double_list **node, char *data);
void	add_node_last(t_double_list **node, char *data);
void	free_list(t_double_list *head);
void	convert_space_buff(void);
void	print_linked_list(t_double_list *node);

int		bt_echo(int nb, char **args);
void	bt_env(void);
int		bt_export(char *arg);
int		bt_pwd(void);
int		bt_cd(char *path);
int		bt_unset(char *name);
t_token	*new_token(char *data);

void	load_stacks(t_token **dst, t_token *data);
void	add_node_last_token(t_token **node, char *data);
void	add_node_middle_token(t_token **node, char *data);
void	free_token(t_token **head);
t_stacks	*build_stack(t_token *tokens);

void	free_stacks(t_stacks **stacks);

int 	is_builtin(t_stacks *cmd);
char	*clear_quotes(char *str);
char	*ft_strchr_token(char *s, int c);
char	*ft_strnstr_token(char *str, char *to_find, size_t n);
void	classify_token(t_token *token);
void	normalize_token(t_token *token);
void	normalize_redirect(t_token *tokens);
//char	*resolve_dollar(int *position);
//char	*resolve_single_quotes(int *position);
//char	*resolve_double_quotes(int *position);
//void	normalize_quotes(void);
t_token	*tokenization_cmd(t_token *tokens);

int		is_valid_input(t_token *cursor);
int		is_valid_out(t_token *cursor);
int		is_valid_heredoc(t_token *cursor);
int		is_valid_out_append(t_token *cursor);
int		is_valid_pipe(t_token *cursor);
int		parse_tkn(t_token *tokens);

void	sig_handle(int signum);
void	handle(int i);
int		ft_get_dir(char *str);
void	init_dir(t_prompt *prompt);
void	free_dir(t_core_var *core);
void	prompt(void);

int		parse_tkn(t_token *tokens);
int		is_valid_input(t_token *cursor);
int		is_valid_out(t_token *cursor);
int		is_valid_heredoc(t_token *cursor);
int		is_valid_out_append(t_token *cursor);
int		is_valid_pipe(t_token *cursor);

void	free_double(char **str);

char	*catch_var(char *name_var);
char	*ft_strjoin_free1(char *s1, char const *s2, size_t size2);

char	*resolve_single_quotes(char *str);
char	*resolve_dollar(char *str);
char	*resolve_double_quotes(char *str);
int		resolve_string(t_token **tokens);

void	open_file(t_stacks *stacks);
char	**load_path(void);
int		is_valid(t_token *cmd);
char	**build_cmd(t_stacks *stack, int id);
int		here_doc(t_stacks *stacks, t_token *tokens,int select_input);
int		amount_pipe(t_stacks *stacks);
//void	here_doc(t_stacks *stacks, t_token *tokens);
//int		number_tokens(t_stacks *stack);
void	execute(t_stacks *stacks, t_token *tokens);

char	*cat_var(char *env, int len_name_var);
int		search_var(char *var, char *env);
#endif /*MINISHELL_H*/
