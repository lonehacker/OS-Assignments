#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
int main (int argc, char *argv[])
{
   int pid;
     if((pid=fork()) == 0)
{
   
      execlp("xterm","xterm","-hold","-e","./sort1",argv[1],NULL);
      
 }  
   exit(0);
}
