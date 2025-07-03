/*Here is an example of multitasking  in linux like systems using fork() :
fork() is a system call used to create a new process by duplicating the calling process.
When a process calls fork(), the operating system creates a new child process that is a copy of the parent process. Both processes will then continue executing independently from the point where fork() was called.
Here is a simple program that generates 16 identical processes.

Because each child also continues the for loop, this causes exponential process creation unless managed carefully.
Here's how:

    On first iteration: 1 process creates 1 child → 2 total

    On second iteration: both processes fork → 4 total

    On third iteration: 4 fork → 8

    ...

    By the 16th iteration, up to 2^16 = 65,536 processes** could be created if no child exits early.

But in this code, the child calls exit(0); after printing, so it does not continue the loop, which avoids explosion.

Therefore, only the parent continues looping, and creates exactly 16 children.*/


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

