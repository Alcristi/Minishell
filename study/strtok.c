#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./include/libft.h"

int main(int argc, char *argv[])
{
	char **tmp;
	tmp = ft_split(argv[1],' ');
	int i;
	i  =0;
	while(tmp[i])
	{
		printf("%s\n",tmp[i]);
		i++;
	}
}
