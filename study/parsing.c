#include <stdio.h>
#include "../include/libft.h"
char *x;
char *check[] = {"<","<<",">",">>","|"};

int parseProduct()
{

}
int parseFactor()
{
	if(*x >= '0' && *x<= '9')
	{
		return *x++ - '0';
	}
	else
	{
		printf("expected digit but found %c\n",*x);
	}
}
void print_list(char **list)
{
	int i;

	i = 0;
	while(list[i])
	{
		printf("%s\n",list[i]);
		i++;
	}
}

char	*ft_strchr_token(char *s, int c)
{
	int	i;

	char *str;
	i = 0;
	if (c == '\0')
		return ((char *) s + ft_strlen(s));
	if (c > 256)
		c -= 256;
	while (s[i])
	{
		if (s[i] == c)
		{
			//s[i] = '\0';
			str = ft_strdup(&s[i + 1]);
			s[i + 1] = '\0';
			return (str);
		}
		i++;
	}
	return (0);
}


int main(void)
{
	x = ft_strdup("amigo");

	printf("%s\n%s\n",ft_strchr_token(x,'m'),x);
	//print_list(check);
	return 0;
}
