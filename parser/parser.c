#include"../../include/libft.h"
#include <stdio.h>
typedef struct s_file
{
	char	*file;
	int		fd;
}t_file;

typedef struct s_simple_comand
{
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
	int					pipe;
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

void replace_into_quotes(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{

	}
}

char* clear_quotes(char **str)
{
	if(str[0][0] == '\"')
		return ft_strtrim(str[0],"m");
	else if(str[0][0] == 'a')
		return ft_strtrim(str[0],"'");
	return ft_strdup("");

}

int	main(int argc, char *argv[],char ** env)
{
	char **tmp;
	char *test = ft_strdup(argv[1]);
	printf("%s\n",test);
	printf("%s\n",clear_quotes(&test));
}

// int	i;
	// char**path;

	// i = 0;
	// path = get_path(env);
	// if(check_if_comand_exist(path,argv[1]))
	// 	printf("1\n");
	// free_double_point(path);

