#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <time.h>
#include "libmsg.h"

#define wait(s) semop(s, &pop, 1)  
#define signal(s) semop(s, &vop, 1)
int registr(char *pname)
{
  if(strlen(pname) > 10)
    return -3;
  int reg_seg,sem_reg,nregs;
  struct sembuf pop,vop;
  struct reg *regs;
  int *no_regs;
  key_t k2,k3,k5;
  int flag = 0,is_dup=0,i;
  k2 = ftok("/usr/local",'B');
  k3 = ftok("/usr/local",'C');
  k5 = ftok("/usr/local",'T');
  reg_seg = shmget(k2,100*sizeof(struct reg),0777|IPC_CREAT);
  nregs = shmget(k3,sizeof(int),0777|IPC_CREAT);
  sem_reg = semget(k5,1,0777|IPC_CREAT); 
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
    
  wait(sem_reg);
  regs = (struct reg *)shmat(reg_seg,0,0);
  no_regs = (int *)shmat(nregs,0,0);
  if(*no_regs<100)
    {
      flag = 1;
      for(i=0;i<100;i++)
	{
	  if(regs[i].valid == 1)
	    {
	      if((strcmp(regs[i].p_name,pname)== 0) || (regs[i].pid == getpid())) 
		is_dup = 1;
	    }
	}
      if(!is_dup){
        for(i=0;i<100;i++)
	{
	  if(regs[i].valid == 0)
	    {
	      strcpy(regs[i].p_name, pname);
	      regs[i].pid = getpid();//PID of calling process
	     
	      *no_regs = *no_regs + 1;
              regs[i].valid = 1;
	      flag = 1;
              break;
	    }
	}
      }
    }
  shmdt(no_regs);
  shmdt(regs);
  signal(sem_reg);
  if(is_dup == 1)
    return -2;
  else if (flag == 0)
    return -1;
  else if (flag == 1)
    return 1;
  return -3;
}
int sendmessage (char *recname, char *mesg)
{
  if(strlen(recname)>10)
    return -3;
  if(strlen(mesg)>100)
    return -3;
  int msg_seg,reg_seg,sem_msg,sem_reg,nregs;
  struct sembuf pop,vop;
  struct str_message *msgs;
  struct reg *regs;
  int *no_regs;
  key_t k1,k2,k3,k4,k5;
  int is_reg=0,i, is_send=0;
  int senderpid = getpid();
  char sendername[10];
 
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
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
  wait(sem_reg);
  regs = (struct reg *)shmat(reg_seg,0,0);
  no_regs = (int *)shmat(nregs,0,0); 
  for(i=0;i<100;i++)
    {
      if(regs[i].pid==senderpid)
	{
	  is_reg =1;
	  strcpy(sendername,regs[i].p_name);
	}
    } 
  shmdt(no_regs);
  shmdt(regs);
  signal(sem_reg);
  if(is_reg)
    {
      wait(sem_msg);
      msgs = (struct str_message *)shmat(msg_seg,0,0);
      for(i=0;i<100;i++)
	{
	  if(msgs[i].valid == 0)
	    {
	      strcpy(msgs[i].sender,sendername);
	      strcpy(msgs[i].recipient,recname);
	      strcpy(msgs[i].message,mesg);
	      msgs[i].time = time(NULL);
	      msgs[i].valid = 1;
	      is_send = 1;
	      break;
	    }
	}
      shmdt(msgs);
      signal(sem_msg);
    }
  else
    return -1;
  if(is_send ==1)
    return 1;
  else if (is_send == 0)
    return -2;
  return -3;
}
int listmessages()
{

  int msg_seg,reg_seg,sem_msg,sem_reg,nregs;
  struct sembuf pop,vop;
  struct str_message *msgs;
  struct reg *regs;
  key_t k1,k2,k3,k4,k5;
  int i,found=0,msgcount=0;
  int currpid = getpid();
  char rec[10];
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
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
  wait(sem_reg);
  regs = (struct reg *)shmat(reg_seg,0,0);
  for(i=0;i<100;i++)
    {
      if(regs[i].valid ==1)
	{
	  if(regs[i].pid == currpid)
	    {
	      strcpy(rec,regs[i].p_name);
	      found =1;
	    }
	}
    }
  shmdt(regs);
  signal(sem_reg);
  wait(sem_msg);
  msgs = (struct str_message *)shmat(msg_seg,0,0);
  if(found)
    {
      for(i=0;i<100;i++)
	{
	  if(msgs[i].valid == 1)
	    {
                
	      if(strcmp(msgs[i].recipient, rec) == 0)
		{
		  printf("%s \t %s\n",msgs[i].sender,msgs[i].message);
		  msgcount++;
		}
	    }
	}
    }
  shmdt(msgs);
  signal(sem_msg);
  if(found)
    return msgcount;
  else
    return 0;
  return -1;
}
int readmessage(struct str_message *buffer )
{

   
  int msg_seg,sem_msg,reg_seg,nregs,sem_reg;
  struct sembuf pop,vop;
  struct str_message *msgs;
  struct reg *regs;
  key_t k1,k2,k3,k4,k5;
  int i,found=0,msgcount=0;
  int currpid = getpid();
  int mint=0,minid=0;
  char rec[10];
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
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
  wait(sem_reg);
  regs = (struct reg *)shmat(reg_seg,0,0);
  for(i=0;i<100;i++)
    {
      if(regs[i].valid ==1)
	{
	  if(regs[i].pid == currpid)
	    {
	      strcpy(rec,regs[i].p_name);
	      found =1;
	    }
	}
    }
  shmdt(regs);
  signal(sem_reg);
  wait(sem_msg);
  msgs = (struct str_message *)shmat(msg_seg,0,0);
  if(found)
    {
      for(i=0;i<100;i++)
	{
	  if(msgs[i].valid == 1)
	    {
	      if(strcmp(msgs[i].recipient, rec) == 0)
		{
		  if(mint<msgs[i].time)
		    {
		      mint = msgs[i].time;
                      minid = i;
		    }
		}
	    }
	}
    }
  strcpy(buffer->sender, msgs[minid].sender);
  strcpy(buffer->recipient, msgs[minid].recipient);
  strcpy(buffer->message, msgs[minid].message);
  buffer->time = msgs[minid].time;
  msgs[minid].valid = 0;
  msgcount = 1;
  shmdt(msgs);
  signal(sem_msg);
  if(found)
    { 
      if(msgcount)
	return 1;
      else
	return 0;
    }
  else
    return 0;
  return -1;


}
int deregister()
{

  int reg_seg,sem_reg,nregs;
  struct sembuf pop,vop;
  struct reg *regs;
  int *no_regs;
  key_t k2,k3,k5;
  int i;
  int curid = getpid();
  k2 = ftok("/usr/local",'B');
  k3 = ftok("/usr/local",'C');
  k5 = ftok("/usr/local",'T');
  reg_seg = shmget(k2,100*sizeof(struct reg),0777|IPC_CREAT);
  nregs = shmget(k3,sizeof(int),0777|IPC_CREAT);
  sem_reg = semget(k5,1,0777|IPC_CREAT); 
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
    
  wait(sem_reg);
  regs = (struct reg *)shmat(reg_seg,0,0);
  no_regs = (int *)shmat(nregs,0,0);
  for(i=0;i<100;i++)
    {
      if(regs[i].pid == curid)
	{
	  regs[i].valid = 0;
	  *no_regs = *no_regs -1;
	}
    }
  shmdt(no_regs);
  shmdt(regs);
  signal(sem_reg);

}
