/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcrist@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 23:21:27 by esilva-s          #+#    #+#             */
/*   Updated: 2022/10/04 20:07:02 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

# define INTERRUPT_WITHOUT_CMD 33280
# define FILE_ERROR_WITHOUT_CMD 256
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define IS_DIRECTORY 126
# define INTERRUPT_SIG_INT 130
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
	int				quotes;
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
	char	*path;
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
	int				fd_stdin;
	int				fd_stdout;
	int				fd_in;
	int				fd_out;
	int				exit_code;
}t_core_var;

typedef struct s_vars
{
	char			*temp;
	char			quotes;
	size_t			index;
	size_t			c_temp;
}	t_vars;

extern t_core_var	*g_core_var;

char			**convert_env_for_string(void);
void			free_two(char **str, t_token **cmd);
void			free_core(void);
void			free_list(t_double_list *head);
void			free_token(t_token **head);
void			free_stacks(t_stacks **stacks);
void			free_dir(t_core_var *core);
void			free_double(char **str);
void			free_exec(t_stacks **stacks, t_token **tokens);
void			sig_handle(int signum);
void			handle(int i);
void			handle_wait(int *pid);
void			handle_redirect(t_stacks *stacks, t_token *tokens,
					int *pid, int count);
void			handle_redirect_pipe(t_stacks *stacks, t_token *tokens,
					int *pid, int count);
void			handle_pipe(int count, int quantity_cmd, int *fd_pipe);
void			handle_here(int i);
void			handle_quit(int i);
int				execute_builtin(char **cmd);
void			execute(t_stacks **stacks, t_token **tokens);
void			exec_with_pipe(t_stacks **stacks,
					t_token **tokens, int quantity_cmd);
void			exec_in_pipe(t_stacks **stacks, t_token **tokens,
					int *pid_child, int count);
void			exec_builtin(t_stacks *stacks, t_token *tokens);
void			exec_here_doc(t_stacks *stacks, t_token *tokens,
					int *pid, int count);
void			exec_here_cmd(t_stacks *stacks, t_token *tokens, int pid);
void			exec_fail(t_stacks **stacks, t_token **tokens,
					char **cmd, char **envp);
void			validate_fork(int pid);
int				is_valid_input(t_token *cursor);
int				is_valid_out(t_token *cursor);
int				is_valid_heredoc(t_token *cursor);
int				is_valid_out_append(t_token *cursor);
int				is_valid_pipe(t_token *cursor);
int				is_valid(t_token *cmd, t_stacks **stacks, t_token **tokens);
int				bt_pwd(void);
int				bt_unset(char *name);
int				bt_export(char **arg);
void			bt_env(void);
int				bt_echo(int nb, char **args);
int				bt_cd(char **path);
int				bt_exit(char **nb);
t_double_list	*new(char *data);
void			add_node_front(t_double_list **node, char *data);
void			add_node_last(t_double_list **node, char *data);
char			convert_space_buff(void);
void			cut_quotes(char *str, t_double_list **aux);
void			print_linked_list(t_double_list *node);
t_token			*new_token(char *data);
void			load_stacks(t_token **dst, t_token *data);
void			add_node_last_token(t_token **node, char *data);
void			add_node_middle_token(t_token **node, char *data);
t_stacks		*build_stack(t_token *tokens);
char			*clear_quotes(char *str);
char			*ft_strchr_token(char *s, int c);
char			*ft_strnstr_token(char *str, char *to_find, size_t n);
void			classify_token(t_token *token);
void			normalize_token(t_token *token);
void			normalize_redirect(t_token *tokens);
t_token			*tokenization_cmd(t_token *tokens);
int				ft_get_dir(char *str);
void			init_dir(t_prompt *prompt);
void			prompt(void);
int				parse_tkn(t_token *tokens);
char			*catch_var(char *name_var);
char			*ft_strjoin_free1(char *s1, char const *s2, size_t size2);
char			*resolve_single_quotes(char *str);
char			*resolve_dollar(char *str);
char			*resolve_double_quotes(char *str);
int				resolve_string(t_token **tokens);
void			open_file(t_stacks *stacks);
char			**load_path(char *cmd, t_stacks **stacks, t_token **tokens);
char			**build_cmd(t_stacks **stack, t_token **tokens, int id);
int				here_doc_pipe(t_stacks *stacks, t_token *tokens,
					int is_priority, int *pid);
int				here_doc(t_stacks *stacks, t_token *tokens, int is_priority);
int				amount_pipe(t_stacks *stacks);
int				is_builtin(t_stacks *cmd);
void			copy_fd(int font, int dest);
void			parent(int pid, t_stacks *stacks);
int				select_stdin(t_token *tokens);
void			file_error(char *str);
char*			open_out(t_stacks *stacks);
char			*cat_var(char *env, int len_name_var);
int				search_var(char *var, char *env);
int				check_cmd(char *str, t_stacks *stacks, t_token *tokens);
char			*expansion_exit_code(void);
char			*mount_message_eof(char *s);
void			exec_here_builtin(t_stacks *stacks,
					t_token *tokens, int pid);
void			handle_redirect_builtin(t_stacks *stacks,
					t_token *tokens, int *pid, int count);
void			exit_builtin_in_pipe(char **cmd, t_stacks **stacks,
					t_token **tokens, int exit_code);
int				is_builtin_cmd(char *cmd);
void			free_exit(t_stacks **stacks, t_token **tokens);
void			error_message(char *str, char *message_error);
void			system_error(int number_err, char *str);
void			validate_fork(int pid);
int				has_quotes(char *str);
void			init_point(char ***cmd, char ***envp);
int				check_args(char **path);
char			*open_in(t_stacks *stacks);
#endif
