#include <stdio.h>
#include <syscall.h>

//compile it, copy to the pintos virtual disk an call this program with arguments using the command
// pintos --qemu -- run lab3test1
//if lab 3 is correctly implemented this program should call itself in an infinite loop and spawn itself as a child process indefinetly
//the PID of each child process created should be printed to the screen
// In order to see if your implementation works check if the PID of each new  child process is incremented as it should

int
main (int argc, char *argv[])
{
  int pid=exec("lab3test1");
  printf("Child process ID: %d",pid);

  return EXIT_SUCCESS;
}
