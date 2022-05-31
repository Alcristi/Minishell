#include "../include/parser.h"
#include <stdio.h>
typedef struct s_file
{
	char	*file;
	int		fd;
}t_file;

typedef struct s_simple_comand
{
	int		numbers_of_available_arguments;
	int		numbers_of_arguments;
	char	**arguments;
	char	*comand;
}t_simple_comand;

typedef struct s_comand
{
	int					numbers_of_simple_comands;
	t_simple_comand		**t_simple_comands;
	t_file				input_file;
	t_file				out_file;
	t_file				err_file;
}t_comand;


int check_if_comand_exist(char **path, char *comand)
{
	int	i;
	char *aux_path;
	i = 0;

	while(path[i])
	{
		aux_path = ft_strjoin(path[i],comand);
		if(!access(aux_path,F_OK | X_OK))
		{
			free(aux_path);
			return (1);
		}
		free(aux_path);
		i++;
	}
	return (0);
}

char **get_path(char **env)
{
	int		i;
	char	*aux_path;
	char	**path;

	i = 0;
	while(ft_strncmp("PATH",env[i],4))
		i++;
	aux_path = ft_strdup(env[i] + 5);
	path = ft_split(aux_path,':');
	free(aux_path);
	return (path);
}

void	free_double_point(char **str)
{
	int	i;

	i = -1;
	while(str[++i])
		free(str[i]);
	free(str);
}

int	main(int argc, char **argv,char ** env)
{
	int	i;
	char**path;

	i = 0;
	path = get_path(env);
	if(check_if_comand_exist(path,argv[1]))
		printf("1\n");
	free_double_point(path);
}

