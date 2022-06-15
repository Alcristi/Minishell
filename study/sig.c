#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include <unistd.h>


void handler(int num)
{
	printf("fechou\n");
	exit(1);
}


void handler1(int num)
{
	printf("fechou1\n");
	exit(1);
}
int main(void)
{
	signal(SIGINT,handler);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGSTOP,handler1);



	while(1)
	{
			printf("Aq\n");
			sleep(1);
	}
}
