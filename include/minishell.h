
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


t_double_list	*new(char* data);
void	add_node_front(t_double_list **node, char* data);
void	add_node_last(t_double_list **node, char* data);
void	free_list(t_double_list *head);
void print_linked_list(t_double_list *node);
