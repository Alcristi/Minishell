
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>

#include<readline/readline.h>
#include<readline/history.h>

#include "./libft.h"

#define GREEN "\e[1;32m"
#define PURPLE "\e[1;35m"
#define BLUE "\e[1;34m"
#define END "\e[0m"
#define TRUE 1
#define FALSE 0



typedef struct s_token
{
	char *str;
	int is_pipe;
	int is_input;
	int is_output;
	int is_heredoc;
	int is_out_append;
	int is_cmd;
	int is_arg;
	int single_quotes;
	int double_quotes;
	struct s_token *next;
	struct s_token *previus;
} t_token;


typedef struct s_double_list
{
	char	*data;
	struct s_double_list *next;
	struct s_double_list *previus;
}t_double_list;

typedef struct s_dir
{
	char	*current;
	char	*path_current;
	int		size_path;
	int		position;
}t_dir;

typedef struct s_prompt
{
	char	*user;
	t_dir	 dir;
	char	*prompt;
}t_prompt;

typedef struct s_core_var
{
	char			*buff;
	t_prompt		prompt;
	t_double_list	*env;
}t_core_var;

extern t_core_var	*g_core_var;

t_double_list	*new(char* data);
void	add_node_front(t_double_list **node, char* data);
void	add_node_last(t_double_list **node, char* data);
void	free_list(t_double_list *head);
void print_linked_list(t_double_list *node);

t_token *new_token(char *data);
void add_node_last_token(t_token **node, char *data);
void add_node_middle_token(t_token **node,char *data);
void	free_token(t_token *head);

char *clear_quotes(char *str);
char	*ft_strchr_token(char *s, int c);
char	*ft_strnstr_token(char *str, char *to_find, size_t n);
void	classify_token(t_token *token);
void	normalize_token(t_token *token);
void	normalize_redirect(t_token *tokens);
char	*resolve_dollar(int *position);
char *resolve_single_quotes(int *position);
char *resolve_double_quotes(int *position);
void	normalize_quotes();
t_token *parse_cmd(t_token *tokens);

int ft_get_dir(char *str);
void init_dir(t_prompt *prompt);
void free_dir(t_core_var *core);
void prompt(void);

void	free_double(char **str);
