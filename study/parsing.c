#include <stdio.h>
#include "../include/libft.h"
#include "../include/minishell.h"

void new_exec(char **envp)
{
	char *const argv1[] = {"/usr/bin/cat","/dev/urandom",NULL};
	char *const argv2[] = {"/usr/bin/head","-1",NULL};
	int status;
	int array[2];
	int pp[2];
	int salve_fd[2];

	salve_fd[0] = dup(STDIN_FILENO);
	salve_fd[1] = dup(STDOUT_FILENO);
	pipe(pp);
	for(int i = 0; i < 2; i++)
	{
		array[i] = fork();
		if(array[i] == 0)
		{
			if(i == 0)
			{
				close(pp[0]);
				dup2(pp[1], STDOUT_FILENO);
				execve(argv1[0],argv1,envp);
			}
			else
			{
				close(pp[0]);
				close(pp[1]);
				execve(argv2[0],argv2,envp);
			}
		}
		if(i == 0 && array[0])
		{
			close(pp[1]);
			close(pp[0]);
			dup2(pp[0], STDIN_FILENO);
		}
	}
	dup2(salve_fd[0],STDIN_FILENO);
	dup2(salve_fd[1],STDOUT_FILENO);

	close(salve_fd[0]);
	close(salve_fd[1]);

	waitpid(array[0],&status,0);
	waitpid(array[1],&status,0);
}


int main(int argc, char **argv, char **envp)
{
	new_exec(envp);
	return 0;
}
