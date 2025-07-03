#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>


int main(void)
{
	pid_t npid;
	int i;
for(i=0;i<16;i++)
{
	
	npid = fork();
	if(npid<0) 
	{
		fprintf(stderr,"Error No. %d in fork()!\n",errno);
		exit(1);
	}

	if(npid ==0){
	
		printf("i=%d, Process PPID=%ld -> PID = %ld \n",i,(long)getppid(),(long)getpid());
		pause();
		exit(0);
		}
	
}
sleep(2);
return 0;
}

