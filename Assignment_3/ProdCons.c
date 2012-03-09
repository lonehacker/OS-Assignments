#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)
#define SUM 1275
int main()

{
  int status,id1,id2,id3;
  int *buff,*sum_arr,*start_arr;
  int buffer_write,buffer_read,mutex;
  int buffer,sum,start;
  struct sembuf pop, vop ;
  buffer_write = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  buffer_read = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  mutex = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	
  semctl(buffer_write, 0, SETVAL, 20);
  semctl(buffer_read, 0, SETVAL, 0);
  semctl(mutex, 0, SETVAL, 1);
	
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
  
  buffer = shmget(IPC_PRIVATE,20*sizeof(int),0777|IPC_CREAT);   //shared buffer
  sum = shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);     //SUM variable
  start = shmget(IPC_PRIVATE,sizeof(int),0777|IPC_CREAT);    //read end of buffer
  int end = 0; 
  int temp = 0;
  if((id1=fork()) == 0)
  {
       //Producer
       end = 0;
       int i;
       for(i=0;i<50;i++)
       {
         P(buffer_write); //wait for space to write
         P(mutex);        // 
         buff = (int *)shmat(buffer,0,0);
         buff[end] = i+1;
         end++;
         printf("Producer wrote %d , position = %d\n",buff[end-1],end-1);
         if (end == 20)
             end = 0;
         shmdt(buff);
         V(mutex);
         V(buffer_read); //Signal that consumer can read
       }
       printf("Exiting Producer\n");
       exit(0);
  }
  else if((id1=fork()) == 0)
  {
      //Consumer 1
      while(1)
      {
        P(buffer_read); //wait for read permission
        P(mutex); //wait for other processes
        buff = (int *)shmat(buffer,0,0);
        sum_arr = (int *)shmat(sum,0,0);
        start_arr = (int *)shmat(start,0,0);
        if(*sum_arr == SUM)
        {
          printf("Exitting Consumer 1 \n");
          V(buffer_read); //other consumer can read (prevent deadlock)
          V(mutex);
          exit(1);
        }
        temp = buff[*start_arr];
        *start_arr = *start_arr + 1;
        *sum_arr += temp;
         printf("Consumer 1 read %d sum= %d\n",temp,*sum_arr);
        if(*start_arr == 20)
           *start_arr = 0;
        if(*sum_arr == SUM)
        {
          printf("Exitting Consumer 1 \n");
          V(buffer_read); //other consumer can read (prevent deadlock)
          V(mutex);
          exit(1);
        }
        shmdt(buff);
        shmdt(sum_arr);
        shmdt(start_arr);
        V(mutex);
        V(buffer_write); //Signal that one element has been consumed and can be written
        
      }

  }
  else if((id1=fork()) == 0)
  {
      //Consumer 2
      while(1)
      {
        P(buffer_read); //wait for read permission
        P(mutex); //wait for other processes
        buff = (int *)shmat(buffer,0,0);
        sum_arr = (int *)shmat(sum,0,0);
        start_arr = (int *)shmat(start,0,0);
        if(*sum_arr == SUM)
        {
          printf("Exitting Consumer 2 \n");
          V(buffer_read); //other consumer can read (prevent deadlock)
          V(mutex);
          exit(1);
        }
        temp = buff[*start_arr];
        *start_arr = *start_arr + 1;
        *sum_arr += temp;
        printf("Consumer 2 read %d sum= %d\n",temp,*sum_arr);
        //temp = *sum_arr;
        if(*start_arr == 20)
           *start_arr = 0;
        if(*sum_arr == SUM)
        {
          printf("Exitting Consumer 2 \n");
          V(buffer_read); //other consumer can read (prevent deadlock)
          V(mutex);
          exit(1);
        }
        shmdt(buff);
        shmdt(sum_arr);
        shmdt(start_arr);
        V(mutex);
        V(buffer_write); //Signal that one element has been consumed and can be written
        
      }

  }
  else
  {
     //Parent
     waitpid(id1, &status, 0);
     waitpid(id2, &status, 0);
     waitpid(id3, &status, 0);
     sum_arr = (int *)shmat(sum,0,0);
     printf("\n\n\nThe Parent prints sum= %d\n\n\n",*sum_arr);
     shmdt(sum_arr);	
     shmctl(buffer, IPC_RMID, 0);
     shmctl(sum, IPC_RMID, 0);
     shmctl(start, IPC_RMID, 0);
     semctl(buffer_read, 0, IPC_RMID, 0);
     semctl(buffer_write, 0, IPC_RMID, 0);
     semctl(mutex, 0, IPC_RMID, 0);

  }
return 0;
}
