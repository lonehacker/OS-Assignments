#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include<sys/ipc.h>
int main()
{
 
 char temp[200];
 char command[200];
 char tmp;int i=0;char out[100];int status;
 struct dirent *myfile;
 int mkdir_stat;
 struct stat sb;
 DIR *dp = NULL;
 int nargs=0;char **args;char *temp2;int inpt=0,otpt=0;int npipes=0;int k;
 
 do
{  
 nargs =0;inpt=0;otpt=0;npipes=0;k=0;
 printf("myshell>");
 fgets(command,200,stdin);
 if(strcmp(command,"\n")==0)
{ continue;}
 command[strlen(command)-1]='\0';
 char is_and = command[strlen(command)-1];
 strcpy(temp,command);
 temp2=strtok(command," ");nargs++;
 while((temp2=strtok(NULL," "))!=NULL)
  { nargs++;
    if(strcmp(temp2,"|")==0)
   {
     npipes++;     
   }
  }
 args = (char **)malloc(nargs * sizeof(char *));
 int pipes[npipes];
 args[0] = strtok(temp," ");      
 for(i=1;i<nargs;i++)
 {
   args[i] = strtok(NULL," ");
   if(strcmp(args[i],"<")==0)
    inpt=i;
   if(strcmp(args[i],">")==0)
    otpt=i;
   if(strcmp(args[i],"|")==0)
   {
     pipes[k] = i; k++;  
   }
 }
 


  if(strcmp(args[0],"pwd")==0)
    {
     if(getcwd(out,100)){
	printf("%s\n",out);
     }
    }				
  else if(strcmp(args[0],"ls")==0)
   {
    
    
 
  if(getcwd(out,100))
   {
    dp = opendir(out);
    while(myfile = readdir(dp))
        if(strcmp(myfile->d_name,".")!=0 && strcmp(myfile->d_name,"..")!=0)
        if(args[1] == NULL)
         printf("%s ",myfile->d_name);
        else if(strcmp(args[1],"-l")==0)
        {
          if (stat(myfile->d_name, &sb) == -1) {
            printf("Error in getting file info");
        
           }
          else
          {
            printf("%lo(octal) ",
            (unsigned long) sb.st_mode);
             printf("%ld ", (long) sb.st_nlink);
            
           printf("%ld %ld ",
            (long) sb.st_uid, (long) sb.st_gid);
           printf("%lld ",
            (long long) sb.st_size);
             printf("%s ", ctime(&sb.st_mtime));
            printf("%s \n",myfile->d_name);
          } 
        }
        
    }
    printf("\n");
   }

  else if(strcmp(args[0],"cd")==0)
  {
	
        if(chdir(args[1])==-1){
	printf("Error occured during execution of command\n");
	}
  }
  else if(strcmp(args[0],"mkdir")==0)
 {	
        mkdir_stat = umask(0);
        if((mkdir_stat = mkdir(args[1],0777))!= 0)
        
         printf("mkdir failed");
  }
  else if(strcmp(args[0],"rmdir")==0)
  {	
        
        if((mkdir_stat = rmdir(args[1]))!= 0)
        
         printf("rmdir failed");
  }
 else if(strcmp(args[0],"cp")==0)
  {
   
   if(args[1]==NULL)
	printf("file not there\n");
   else
   {
     char* pat = getenv("PATH");
     strcat(pat,":.");
     setenv("PATH",pat,1);
     int fin,fout;	
     fin = open(args[1],O_RDONLY);
     if(fin >= 0)
	{
	 fout = open(args[2],O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	 if(fout >= 0)
	 {
	  char buf[100];
	  int tmp;
						
	  while((tmp=read(fin,buf,100))!=0)
		write(fout,buf,tmp);
	}
	}
    }
}else{
       if(npipes == 0)
       {
         
       int p = fork();
       if(p==0)
        {
       	int ifd,ofd;
       	char *env = getenv("PATH");
       	strcat(env,":.");
       	setenv("PATH",env,1);
       	char *input;
       	char *output;
			
	if(inpt!=0)
	 input=args[inpt+1];
	
	if(otpt!=0)
	 output=args[otpt+1];

	if(inpt!=0 && otpt!=0)
	{
          ifd = open(input, O_RDONLY);
          ofd = open(output, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
          close(0);    /* Close the file descriptor for stdin */
          close(1);    /* Close the file descriptor for stdout */
	  dup(ifd);    /* Duplicate ifd at the lowest-numbered unused descriptor */	
  	  dup(ofd);    /* Duplicate ofd at the lowest-numbered unused descriptor */
  	}else if(inpt!=0)
	{
         ifd = open(input, O_RDONLY);
         close(0);    /* Close the file descriptor for stdin */
	 dup(ifd);    /* Duplicate ifd at the lowest-numbered unused descriptor */
	}else if(otpt!=0){
        ofd = open(output, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   	close(1);    /* Close the file descriptor for stdout */
	dup(ofd);    /* Duplicate ofd at the lowest-numbered unused descriptor */
	}
        if(inpt ==0 && otpt==0)									
	{
	 execvp(args[0],args);
	 exit(0);
	}
	else
	{
	 execlp(args[0],args[0],(char *)NULL);
	 exit(0);
	}
        
       }else{ //parent process
        if(is_and != '&')       
         waitpid(p,&status,0);
       
      }      
      }else{ // pipes
        char **args1,**args2,**args3,**args4,**args5;int j=0;
        args1 = (char **)malloc(nargs * sizeof(char *));
        args2 = (char **)malloc(nargs * sizeof(char *));
        args3 = (char **)malloc(nargs * sizeof(char *));
        args4 = (char **)malloc(nargs * sizeof(char *));
        args5 = (char **)malloc(nargs * sizeof(char *));
        if (npipes == 1)
        {
          j=0;
          for(i=0;i<pipes[0];i++)
           args1[i] = args[i];
          args1[i] = NULL;
          for(i=pipes[0]+1;i<nargs;i++)
          {
            args2[j] = args[i];j++;
          }
          args2[j] = NULL;
          int fd[2];
          pipe(fd);
          int id1 = fork();
          if(id1==0)
          {  printf("Child 1\n");
             close(fd[0]);
             dup2(fd[1],1);
             close(fd[1]);
             execvp(args1[0],args1);
             printf("Executed c 1\n");
             exit(0);
          }else{
             
             int id2 = fork();
             if(id2 == 0) //Within second child process b.out
             {
               printf("Child 2\n");
               close(fd[1]);
               dup2(fd[0],0);
               close(fd[0]);
               execvp(args2[0],args2);
               
              printf("%s\n",args2[0]);
               if(args2[1] == NULL)
                  printf("Correct");
               exit(0);
             }
             else
              {
                int status;
                close(fd[0]);
                close(fd[1]);
                waitpid(id1,&status,0);
                waitpid(id2,&status,0);
                                       
              }
          }
        }
        else if (npipes == 2)
        {
          j=0;
          for(i=0;i<pipes[0];i++)
           args1[i] = args[i];
          for(i=pipes[0]+1;i<pipes[1];i++)
          {
           args2[j] = args[i];j++;
          }
          j=0;
          for(i=pipes[1]+1;i<nargs;i++)
          {
           args3[j] = args[i];j++;
          }



          int fd1[2];
          pipe(fd1);
          int id1 = fork();
          if(id1==0)
          {  
             close(fd1[0]);
             dup2(fd1[1],1);
             close(fd1[1]);
             execvp(args1[0],args1);
             printf("Executed c 1\n");
             exit(0);
          }else{
             int fd2[2];
             pipe(fd2);
             int id2 = fork();
             if(id2 == 0) //Within second child process b.out
             {
               close(fd2[0]);  
               close(fd1[1]);
               dup2(fd1[0],0);
               dup2(fd2[1],1);
               close(fd2[1]);
               close(fd1[0]);
               execvp(args2[0],args2);
             }
             else
             {
               int id3 = fork();
               if (id3 == 0) // c.out
               {
                 close(fd1[0]);
                 close(fd1[1]);
                 close(fd2[1]);
                 dup2(fd2[0],0);
                 close(fd2[0]);
                 execvp(args3[0],args3);

               }
             else
               { 
                int status;
                close(fd1[0]);
                close(fd2[1]);
                close(fd2[0]);
                close(fd1[1]);
                waitpid(id1,&status,0);
                waitpid(id2,&status,0);
                waitpid(id3,&status,0);
              }
          }
        }
        }
        else if (npipes == 3)
        {
          j=0;
          for(i=0;i<pipes[0];i++)
           args1[i] = args[i];
          for(i=pipes[0]+1;i<pipes[1];i++)
          {
            args2[j] = args[i];j++;
          }
          j=0;
          for(i=pipes[1]+1;i<pipes[2];i++)
          {
            args3[j] = args[i];j++;
          }
          j=0;
          for(i=pipes[2]+1;i<nargs;i++)
          {
           args4[j] = args[i];j++;
          }


          int fd1[2];
          pipe(fd1);
          int id1 = fork();
          if(id1==0)
          {  
             close(fd1[0]);
             dup2(fd1[1],1);
             close(fd1[1]);
             execvp(args1[0],args1);
             printf("Executed c 1\n");
             exit(0);
          }else{
             int fd2[2];
             pipe(fd2);
             int id2 = fork();
             if(id2 == 0) //Within second child process b.out
             {
               close(fd2[0]);  
               close(fd1[1]);
               dup2(fd1[0],0);
               dup2(fd2[1],1);
               close(fd2[1]);
               close(fd1[0]);
               execvp(args2[0],args2);
             }
             else
             { int fd3[2];
               pipe(fd3);
               int id3 = fork();
               if (id3 == 0) // c.out
               {
                 close(fd1[0]);
                 close(fd1[1]);
                 close(fd2[1]);
                 dup2(fd2[0],0);
                 dup2(fd3[1],1);
                 close(fd2[0]);
                 close(fd3[1]);
                 execvp(args3[0],args3);

               }
               else{
                    int id4 = fork();
                    if(id4 ==0)
                    {
                       close(fd1[0]);
                       close(fd1[1]);
                       close(fd2[1]);
                       close(fd2[0]);
                       close(fd3[1]);
                       dup2(fd3[0],0);
                       close(fd3[0]);
                       execvp(args4[0],args4);
                    }
                 else
                    { 
                     int status;
                     close(fd1[0]);
                     close(fd2[1]);
                     close(fd2[0]);
                     close(fd1[1]);
                     close(fd3[1]);
                     close(fd3[0]);
                     
                waitpid(id1,&status,0);
                waitpid(id2,&status,0);
                waitpid(id3,&status,0);
                waitpid(id4,&status,0);
              }
          }
        }
        }
        }
        else if (npipes == 4)
        {
          j=0;
          for(i=0;i<pipes[0];i++)
           args1[i] = args[i];
          for(i=pipes[0]+1;i<pipes[1];i++)
          {
           args2[j] = args[i];j++;
          }
          j=0;
          for(i=pipes[1]+1;i<pipes[2];i++)
          {
           args3[j] = args[i];j++;
          }
          j=0;
          for(i=pipes[2]+1;i<pipes[3];i++)
          {
           args4[j] = args[i];j++;
          }
          j=0;
          for(i=pipes[3]+1;i<nargs;i++)
          {
           args5[j] = args[i];j++;
          }
         

          int fd1[2];
          pipe(fd1);
          int id1 = fork();
          if(id1==0)
          {  
             close(fd1[0]);
             dup2(fd1[1],1);
             close(fd1[1]);
             execvp(args1[0],args1);
             printf("Executed c 1\n");
             exit(0);
          }else{
             int fd2[2];
             pipe(fd2);
             int id2 = fork();
             if(id2 == 0) //Within second child process b.out
             {
               close(fd2[0]);  
               close(fd1[1]);
               dup2(fd1[0],0);
               dup2(fd2[1],1);
               close(fd2[1]);
               close(fd1[0]);
               execvp(args2[0],args2);
             }
             else
             { int fd3[2];
               pipe(fd3);
               int id3 = fork();
               if (id3 == 0) // c.out
               {
                 close(fd1[0]);
                 close(fd1[1]);
                 close(fd2[1]);
                 dup2(fd2[0],0);
                 dup2(fd3[1],1);
                 close(fd2[0]);
                 close(fd3[1]);
                 execvp(args3[0],args3);

               }
               else{
                    int fd4[2];
                    pipe(fd4);
                    int id4 = fork();
                    if(id4 ==0)
                    {
                       close(fd1[0]);
                       close(fd1[1]);
                       close(fd2[1]);
                       close(fd2[0]);
                       close(fd3[1]);
                       dup2(fd3[0],0);
                       dup2(fd4[1],1);
                       close(fd3[0]);
                       close(fd4[1]);
                       execvp(args4[0],args4);
                    }else{
                       int id5 = fork();
                       if(id5 == 0)
                       { 
                         close(fd1[0]);
                         close(fd1[1]);
                         close(fd2[1]);
                         close(fd2[0]);
                         close(fd3[1]);
                         close(fd3[0]);
                         close(fd4[1]);
                         dup2(fd4[0],0);
                         close(fd4[0]);
                         
                       execvp(args5[0],args5);

                       }

                      else
                      { 
                     int status;
                     close(fd1[0]);
                     close(fd2[1]);
                     close(fd2[0]);
                     close(fd1[1]);
                     close(fd3[1]);
                     close(fd3[0]);
                     close(fd4[1]);
                     close(fd4[0]);
                     
                waitpid(id1,&status,0);
                waitpid(id2,&status,0);
                waitpid(id3,&status,0);
                waitpid(id4,&status,0);
                waitpid(id5,&status,0);
              }
          }
        }
        }
        }
        }



      }
  }											
}while(strcmp(args[0],"exit")!=0);

}
