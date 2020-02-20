#include <stdio.h>
#include <syscall.h>

//compile it, copy to the pintos virtual disk an call this program with arguments using the command
// pintos --qemu -- run 'lab4test1 arg1 arg2 arg3'
//if lab 4 is correctly implemented the arguments should be printed to the console
// try it wih different arguments and different number of arguments

int
main (int argc, char *argv[])
{

  for(int i=0;i<argc;i++)
	{
	  printf("Parameter: %d: %s \n",i,argv[i]);
	}

  return EXIT_SUCCESS;
}


