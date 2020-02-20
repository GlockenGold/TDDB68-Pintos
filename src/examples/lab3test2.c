#include <stdio.h>
#include <syscall.h>

//compile it, copy to the pintos virtual disk an call this program with arguments using the command
// pintos --qemu -- run lab3test2
//if lab 3 is correctly implemented this program should call printf 5 times, and spawn a child process for each of these calls
//the PID of each child process created should be printed to the console.
// In order to see if your implementation works check if the string "You got it, use your debugging skills during the labs!" is printed 5 times and if the PID of each new child process is incremented as it should


int
main (int argc, char *argv[])
{

  int pid=-1;
  for(int i=0;i<5;i++)
	{
	pid=exec("printf");
  	printf("Child %d process ID: %d\n",i,pid);
	}

  return EXIT_SUCCESS;
}
