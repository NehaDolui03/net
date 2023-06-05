#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
int main()
{
	int x,n,fd1,fd2[2];
	char str1[1024],str2[1024];
	fd1=open("t1.txt",O_RDONLY,0764);
	n=read(fd1,str1,1024);
	pipe(fd2);
	x=fork();
	if(x==1)
	{
		printf("Error in the fork");
		exit(1);
	}
	else if(x==0)
	{
		close(fd2[1]);
		read(fd2[0],str2,1024);
		printf("The message read from pipe is:%s",str2);
	}
	else
	{
		close(fd2[0]);
		write(fd2[1],str1,1024);
	}
}