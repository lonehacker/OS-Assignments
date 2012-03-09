#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <time.h>
#include "libmsg.h"
#define wait(s) semop(s, &pop, 1)  
#define sig(s) semop(s, &vop, 1)

#define checktime 2
#define deletetime 15
/*struct str_message{
  char sender[10];
  char recipient[10];
  int time;
  char message[100];
  int valid;
};
struct reg{
  char p_name[10];
  int pid;
  int valid;
};*/
void clenup(void);
 int msg_seg,reg_seg,sem_msg,sem_reg,nregs;
void cleanup()
{
  semctl(sem_msg, 0, IPC_RMID, 0);	
  semctl(sem_reg, 0, IPC_RMID, 0);	
  shmctl(msg_seg,IPC_RMID,0);	
  shmctl(reg_seg,IPC_RMID,0);	
  shmctl(nregs,IPC_RMID,0);
  exit(0);
}
int main()
{
  struct str_message *msgs;
  struct reg *regs;
  struct sembuf pop,vop;
  int t;
  int *no_regs,i;
  key_t k1,k2,k3,k4,k5;
  k1 = ftok("/usr/local",'A');
  k2 = ftok("/usr/local",'B');
  k3 = ftok("/usr/local",'C');
  k4 = ftok("/usr/local",'S');
  k5 = ftok("/usr/local",'T');
  msg_seg = shmget(k1,100*sizeof(struct str_message),0777|IPC_CREAT);
  reg_seg = shmget(k2,100*sizeof(struct reg),0777|IPC_CREAT);
  nregs = shmget(k3,sizeof(int),0777|IPC_CREAT);
  sem_msg = semget(k4,1,0777|IPC_CREAT);
  sem_reg = semget(k5,1,0777|IPC_CREAT);
  semctl(sem_msg, 0, SETVAL, 1); 
  semctl(sem_reg, 0, SETVAL, 1);
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
  no_regs = (int *) shmat(nregs,0,0);
  signal(SIGINT,cleanup);
  *no_regs = 0; //No of Registrations = 0
  printf("%d",*no_regs);
  shmdt(no_regs);
  regs = (struct reg *)shmat(reg_seg,0,0);
  msgs = (struct str_message *)shmat(msg_seg,0,0);
  for(i=0;i<100;i++)
  {
   msgs[i].valid = 0; //No messages initially
   regs[i].valid =0 ; // No regs initially
  }
      shmdt(regs);	
      shmdt(msgs);
      
  while(1)
    { 
      
      
      wait(sem_msg);
      t = time(NULL);
      msgs = (struct str_message *)shmat(msg_seg,0,0);
      for(i=0;i<100;i++)
      {
        if(msgs[i].valid ==1)
        {
         if(t - msgs[i].time >=deletetime)
         msgs[i].valid = 0;
        }
      
      }
      shmdt(msgs);
      sig(sem_msg);      
      wait(sem_reg);
      regs = (struct reg *)shmat(reg_seg,0,0);
      no_regs = (int *) shmat(nregs,0,0);
      printf("No of registrations = %d\n",*no_regs);
      shmdt(no_regs);
      shmdt(regs);
      sig(sem_reg);
      sleep(checktime);
    }
    
}
