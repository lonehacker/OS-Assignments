#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1) 

int main()
{
	//int shmid_ip,shmid_op,shmid_c;
	//printf("INITIALLY:: shmid_ip = %d,shmid_op = %d,shmid_c = %d\n\n",shmid_ip,shmid_op,shmid_c);
	
	int shmid_ip = shmget(IPC_PRIVATE, (1024*768*3)*sizeof(int), 0777|IPC_CREAT);
	int shmid_op = shmget(IPC_PRIVATE, (1024*768*3)*sizeof(int), 0777|IPC_CREAT);
	int shmid_c = shmget(IPC_PRIVATE, 6*sizeof(int), 0777|IPC_CREAT); //for m,n,no.of items for each child,inc/dec,%tage,termination
	
	printf("INITIALLY:: shmid_ip = %d,shmid_op = %d,shmid_c = %d\n\n",shmid_ip,shmid_op,shmid_c);
		
	int semidc1,semidc2,semidc3,semidc4,semidp1,semidp2,semidp3,semidp4;
	struct sembuf pop, vop ;
	
	semidc1 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semidp1 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semidc2 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semidp2 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semidc3 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semidp3 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semidc4 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semidp4 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	
	semctl(semidc1, 0, SETVAL, 0);
	semctl(semidp1, 0, SETVAL, 0);
	semctl(semidc2, 0, SETVAL, 0);
	semctl(semidp2, 0, SETVAL, 0);
	semctl(semidc3, 0, SETVAL, 0);
	semctl(semidp3, 0, SETVAL, 0);
	semctl(semidc4, 0, SETVAL, 0);
	semctl(semidp4, 0, SETVAL, 0);
	
	pop.sem_num = vop.sem_num = 0;
	pop.sem_flg = vop.sem_flg = 0;
	pop.sem_op = -1 ; vop.sem_op = 1 ;
	
	int id1 = fork();
	if(id1 == 0)
	{
		P(semidc1);
		printf("\n\nInside Child Process1\n\n");
		printf("IN child1:: shmid_ip = %d,shmid_op = %d,shmid_c = %d\n\n",shmid_ip,shmid_op,shmid_c);
		int *input = (int *)shmat(iparray,0,0);
		int *output = (int *)shmat(oparray,0,0);	
		int *cntrl = (int *)shmat(ctl,0,0);
		int limit = cntl[2];
		int ini = 0,i;
		int inc_dec = cntl[3];
		int brightness = cntrl[0];
		//Decrease in intensity by 10%
		printf("Near For Loop\n\n");
		for(i=ini;i<limit;i++)
		{
			if(inc_dec < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
			else if(inc_dec > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
			if(out[i] < 0) out[i] = 0;
			if(out[i] > 255) out[i] = 255;
			printf("The value of %d changed to %d at i = %d\n",inp[i],out[i],i);
		}
		shmdt(inp);
		shmdt(out);
		shmdt(cntl);
		V(semidp1);
	}
	else
	{
		int id2 = fork();
		if(id2 == 0)
		{
			P(semidc2);
			printf("\n\nInside Child Process2\n\n");
			printf("IN child2:: shmid_ip = %d,shmid_op = %d,shmid_c = %d\n\n",shmid_ip,shmid_op,shmid_c);
			int *inp = (int *)shmat(shmid_ip,0,0);
			int *out = (int *)shmat(shmid_op,0,0);	
			int *cntl = (int *)shmat(shmid_c,0,0);
			int limit = 2*cntl[2];
			int ini = 1*cntl[2],i;
			int inc_dec = cntl[3];
			int brightness = cntl[4];
			//Decrease in intensity by 10%
			printf("Near For Loop\n\n");
			for(i=ini;i<limit;i++)
			{
				if(inc_dec < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
				else if(inc_dec > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
				if(out[i] < 0) out[i] = 0;
				if(out[i] > 255) out[i] = 255;
				printf("The value of %d changed to %d at i = %d\n",inp[i],out[i],i);
			}
			shmdt(inp);
			shmdt(out);
			shmdt(cntl);
			V(semidp2);
		}
		else
		{
			int id3 = fork();
			if(id3 == 0)
			{
				P(semidc3);
				printf("\n\nInside Child Process3\n\n");
				printf("IN child3:: shmid_ip = %d,shmid_op = %d,shmid_c = %d\n\n",shmid_ip,shmid_op,shmid_c);
				int *inp = (int *)shmat(shmid_ip,0,0);
				int *out = (int *)shmat(shmid_op,0,0);	
				int *cntl = (int *)shmat(shmid_c,0,0);
				int limit = 3*cntl[2];
				int ini = 2*cntl[2],i;
				int inc_dec = cntl[3];
				int brightness = cntl[4];
				//Decrease in intensity by 10%
				printf("Near For Loop\n\n");
				for(i=ini;i<limit;i++)
				{
					if(inc_dec < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
					else if(inc_dec > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
					if(out[i] < 0) out[i] = 0;
					if(out[i] > 255) out[i] = 255;
					printf("The value of %d changed to %d at i = %d\n",inp[i],out[i],i);
				}
				shmdt(inp);
				shmdt(out);
				shmdt(cntl);
				V(semidp3);	
			}
			else
			{
				int id4 = fork();
				if(id4 == 0)
				{
					P(semidc4);
					printf("Inside Child Process4\n\n");
					printf("IN child4:: shmid_ip = %d,shmid_op = %d,shmid_c = %d\n\n",shmid_ip,shmid_op,shmid_c);
					int *inp = (int *)shmat(shmid_ip,0,0);
					int *out = (int *)shmat(shmid_op,0,0);	
					int *cntl = (int *)shmat(shmid_c,0,0);
					int limit = cntl[0]*cntl[1]*3;
					int ini = 3*cntl[2],i;
					int inc_dec = cntl[3];
					int brightness = cntl[4];
					//Decrease in intensity by 10%
					printf("Near For Loop\n\n");
					for(i=ini;i<limit;i++)
					{
						if(inc_dec < 0) out[i] = (int)((float)inp[i] - (float)(0.01*brightness*inp[i]));
						else if(inc_dec > 0) out[i] = (int)((float)inp[i] + (float)(0.01*brightness*(255-inp[i])));
						if(out[i] < 0) out[i] = 0;
						if(out[i] > 255) out[i] = 255;
						printf("The value of %d changed to %d at i = %d\n",inp[i],out[i],i);
					}
					shmdt(inp);
					shmdt(out);
					shmdt(cntl);
					V(semidp4);
				}
				else
				{
					char option = 'y';
					
					int *shm_inp = (int *)shmat(shmid_ip,0,0);
					int *shm_op = (int *)shmat(shmid_op,0,0);
					int *shm_c = (int *)shmat(shmid_c,0,0);
					
					printf("Do you want to change the intensity of a .pnm image??[Type 'y' or 'n']::");
					scanf("%c",&option);
					
					if(option == 'n' || option == 'N')
					{
						shm_c[5] = 0; //Signalling all children processes to exit	
					
					}
					
					printf("Enter the name of input file::");
					char infile[100];
					scanf("%s",infile);
					
					printf("Enter the name of the output file::");
					char outfile[100];
					scanf("%s",outfile);
					
					printf("Enter percentage change in brightness[integer between -100 and 100]::");
					int brightness;
					scanf("%d",&brightness);
					
					FILE *fr = fopen(infile,"r");
					FILE *fw = fopen(outfile,"w");
					
					int m,n;
					char type[10];
					int size;
		
					fscanf(fr,"%s",type);
					fprintf(fw,"%s\n",type);
					printf("Type::%s\n",type);
		
					fscanf(fr,"%d %d",&m,&n);
					fprintf(fw,"%d %d\n",m,n);
					int array_size = m*n*3;
					printf("m=%d,n=%d,array_size=%d\n",m,n,array_size);
		
					fscanf(fr,"%d",&size);
					fprintf(fw,"%d\n",size);
					printf("Max_pixel value=%d",size);
		
					/*shmid_ip = shmget(IPC_PRIVATE, array_size*sizeof(int), 0777|IPC_CREAT);
					shmid_op = shmget(IPC_PRIVATE, array_size*sizeof(int), 0777|IPC_CREAT);
					shmid_c = shmget(IPC_PRIVATE, 3*sizeof(int), 0777|IPC_CREAT); //for m,n,termination
					*/
					printf("Array Created!!\n");
		
		
					shm_c[0] = m;                        //m
					shm_c[1] = n;                        //n
					shm_c[2] = (m/4)*n*3;                //bound for each array
					//shm_c[3] = ;                       //+1 indicates increase in intensity, -1 decrease in intensity
		
					if(brightness > 0) { shm_c[3] = 1; shm_c[4] = brightness;}
					else { shm_c[3] = -1; shm_c[4] = (-1)*brightness; }
					
					shm_c[5] = 1;                        //control says to continue;		
							
					int i;
					for(i=0;i<array_size;i++) shm_op[i] = 0;
		
					int a,b,c;
					i = 0;
					int count = 0;
		
					while(count < (m*n))
					{
						fscanf(fr,"%d %d %d",&a,&b,&c);
						shm_inp[i] = a;
						shm_inp[i+1] = b;
						shm_inp[i+2] = c;
						i = i+3;
						count++;
					}
		
		
					count = 0;i = 0;
					int counter = 0;
		
					while(count < (m*n))
					{
						printf("Element = %d %d %d\n",shm_inp[i],shm_inp[i+1],shm_inp[i+2]);
						counter++;
						i = i+3;
						count++;
						if(count%m == 0) printf("\n\n");
					}
		
					printf("\nThe no. of times printed = %d\n",counter);
		
					printf("IN Parent:: shmid_ip = %d,shmid_op = %d,shmid_c = %d\n\n",shmid_ip,shmid_op,shmid_c);
	
					V(semidc1);
					V(semidc2);
					V(semidc3);
					V(semidc4);
					
					P(semidp1);
					P(semidp2);
					P(semidp3);
					P(semidp4);
		
					for(i=0;i<(m*n*3);i++)
					{
						printf("Value of output at i=%d :: %d\n",i,shm_op[i]);
						fprintf(fw,"%d ",shm_op[i]);
						if((i%(m*3)==0) && i != 0) fprintf(fw,"\n");
					}
					
					fclose(fr);
					fclose(fw);
		
					shmdt(shm_inp);
					shmdt(shm_op);
					shmdt(shm_c);
		
					shmctl(shmid_ip, IPC_RMID, 0);
					shmctl(shmid_op, IPC_RMID, 0);
					shmctl(shmid_c, IPC_RMID, 0);
					
					semctl(semidc1, 0, IPC_RMID, 0);
					semctl(semidp1, 0, IPC_RMID, 0);
					semctl(semidc2, 0, IPC_RMID, 0);
					semctl(semidp2, 0, IPC_RMID, 0);
					semctl(semidc3, 0, IPC_RMID, 0);
					semctl(semidp3, 0, IPC_RMID, 0);
					semctl(semidc4, 0, IPC_RMID, 0);
					semctl(semidp4, 0, IPC_RMID, 0);
				}
			}		
		}
	}
	return 1;
}
