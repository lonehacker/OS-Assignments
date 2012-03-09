#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


#define wait(s) semop(s, &pop, 1)
#define signal(s) semop(s, &vop, 1)
#define max_row 1024
#define max_col 768
int main()
{

  FILE *fp1,*fp2;
  int *ip,*op,*ctrl;
  int bright;
  int m,n,i,j,k;
  int iparray,oparray,ctl;
  int c1,c2,c3,c4,p1,p2,p3,p4;
  char type[10];
  int size;
  char in[100];
  char out[100];
  int id1,id2,id3,id4;
  int status;
   struct sembuf pop, vop ;
  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;
  iparray = shmget(IPC_PRIVATE,max_row*max_col*3*sizeof(int),0777|IPC_CREAT); 
  oparray = shmget(IPC_PRIVATE,max_row*max_col*3*sizeof(int),0777|IPC_CREAT); 
  ctl = shmget(IPC_PRIVATE,5*sizeof(int),0777|IPC_CREAT); 
  c1 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  c2 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  c3 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  c4 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  
  p1 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  p2 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  p3 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
  p4 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);

  semctl(p1, 0, SETVAL, 0);
  semctl(p2, 0, SETVAL, 0);
  semctl(p3, 0, SETVAL, 0);
  semctl(p4, 0, SETVAL, 0);
  
  semctl(c1, 0, SETVAL, 0);
  semctl(c2, 0, SETVAL, 0);
  semctl(c3, 0, SETVAL, 0);
  semctl(c4, 0, SETVAL, 0);
  if((id1=fork())==0)
    {
      //child 1
      wait(p1);
  
      int *inp = (int *)shmat(iparray,0,0);
      int *out = (int *)shmat(oparray,0,0);	
      int *cntl = (int *)shmat(ctl,0,0);
      if(cntl[4] == 1)
	{
	  shmdt(inp);
          shmdt(out);
          shmdt(cntl);
          exit(0);
	}	
      int brightness = cntl[0];
      for(i=0;i<((cntl[1]/4)*cntl[2]*3);i++)
	{
	  if(cntl[3] < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
	  else if(cntl[3] > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
          if(out[i] < 0) out[i] = 0;
	  if(out[i] > 255) out[i] = 255;
	}
       shmdt(inp);
       shmdt(out);
       shmdt(cntl);
       signal(c1);
    }
  else if((id2=fork())==0)
    {
      //child 2
      
      wait(p2);
  
      int *inp = (int *)shmat(iparray,0,0);
      int *out = (int *)shmat(oparray,0,0);	
      int *cntl = (int *)shmat(ctl,0,0);
      if(cntl[4] == 1)
	{
	  shmdt(inp);
          shmdt(out);
          shmdt(cntl);
          exit(0);
	}	
      int brightness = cntl[0];
      for(i=((cntl[1]/4)*cntl[2]*3);i<((cntl[1]/2)*cntl[2]*3);i++)
	{
	  if(cntl[3] < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
	  else if(cntl[3] > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
          if(out[i] < 0) out[i] = 0;
	  if(out[i] > 255) out[i] = 255;
	}
       shmdt(inp);
       shmdt(out);
       shmdt(cntl);
       signal(c2);
    }
  else if((id3=fork())==0)
    {
      //child 3
      
      wait(p3);
  
      int *inp = (int *)shmat(iparray,0,0);
      int *out = (int *)shmat(oparray,0,0);	
      int *cntl = (int *)shmat(ctl,0,0);
      if(cntl[4] == 1)
	{
	  shmdt(inp);
          shmdt(out);
          shmdt(cntl);
          exit(0);
	}	
      int brightness = cntl[0];
      for(i=((cntl[1]/2)*cntl[2]*3);i<((cntl[1]/4)*cntl[2]*9);i++)
	{
	  if(cntl[3] < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
	  else if(cntl[3] > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
          if(out[i] < 0) out[i] = 0;
	  if(out[i] > 255) out[i] = 255;
	}
       shmdt(inp);
       shmdt(out);
       shmdt(cntl);
       signal(c3);
    }
  else if((id4=fork())==0)
    {
      //child 4
     
      wait(p4);
  
      int *inp = (int *)shmat(iparray,0,0);
      int *out = (int *)shmat(oparray,0,0);	
      int *cntl = (int *)shmat(ctl,0,0);
      if(cntl[4] == 1)
	{
	  shmdt(inp);
          shmdt(out);
          shmdt(cntl);
          exit(0);
	}	
      int brightness = cntl[0];
      for(i=((cntl[1]/4)*cntl[2]*9);i<(cntl[1]*cntl[2]*3);i++)
	{
	  if(cntl[3] < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
	  else if(cntl[3] > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
          if(out[i] < 0) out[i] = 0;
	  if(out[i] > 255) out[i] = 255;
	}
       shmdt(inp);
       shmdt(out);
       shmdt(cntl);
       signal(c4);
    }
  else{
    while(1)
      {
	
        ip = (int *)shmat(iparray,0,0);
	op = (int *)shmat(oparray,0,0);
        ctrl= (int *)shmat(ctl,0,0);
	printf("\nEnter the name of input file::");
	scanf("%s",in);
        if(strcmp(in,"exit")==0)
	  {
	    ctrl[4]=1; 
            signal(p1);
            signal(p2);
            signal(p3);
            signal(p4);
	    waitpid(id1, &status, 0);
	    waitpid(id2, &status, 0);
	    waitpid(id3, &status, 0);
	    waitpid(id4, &status, 0);
	    shmdt(ip);
	    shmdt(op);
	    shmdt(ctrl);
            shmctl(iparray, IPC_RMID, 0);
	    shmctl(oparray, IPC_RMID, 0);
	    shmctl(ctl, IPC_RMID, 0);

	    semctl(c1, 0, IPC_RMID, 0);
	    semctl(p1, 0, IPC_RMID, 0);
	    semctl(c2, 0, IPC_RMID, 0);
	    semctl(p2, 0, IPC_RMID, 0);
	    semctl(c3, 0, IPC_RMID, 0);
	    semctl(p3, 0, IPC_RMID, 0);
	    semctl(c4, 0, IPC_RMID, 0);
	    semctl(p4, 0, IPC_RMID, 0);
            return 0;
	  }
	else{


	  printf("((cntl[1]/4)*cntl[2]*9)\nEnter the name of the output file::");
	  scanf("%s",out);
	  fp1 = fopen(in,"r");
	  fp2 = fopen(out,"w");
	  printf("\nEnter percentage change in brightness[integer between -100 and 100]::");
	  scanf("%d",&ctrl[0]);
	  if(ctrl[0] > 0) { ctrl[3] = 1; }
	  else { ctrl[3] = -1; ctrl[0] = (-1)*ctrl[0]; }
 
	  fscanf(fp1,"%s",type);
	  fprintf(fp2,"%s\n",type);
	
	  printf("Type::%s\n",type);
                       
	  fscanf(fp1,"%d",&ctrl[1]);
	  fscanf(fp1,"%d",&ctrl[2]);
	  fprintf(fp2,"%d %d\n",ctrl[1],ctrl[2]);
	   

	  fscanf(fp1,"%d",&size);
	  fprintf(fp2,"%d\n",size);

	  for(i=0;i<ctrl[1]*ctrl[2]*3;i++)
	    {
	      fscanf(fp1,"%d",&ip[i]);
	    }
          signal(p1);
	  signal(p2);
	  signal(p3);
	  signal(p4);
	  wait(c1);
	  wait(c2);
	  wait(c3);
	  wait(c4);
	  for(i=0;i<(ctrl[1]*ctrl[2]*3);i++)
	    {
	     
	      fprintf(fp2,"%d ",op[i]);
	      if((i%(ctrl[1]*3)==0) && i != 0) fprintf(fp2,"\n");
	    }

	  fclose(fp1);
	  fclose(fp2);
          shmdt(ip);
          shmdt(op);
          shmdt(ctrl);
	}
      }
  }
}
