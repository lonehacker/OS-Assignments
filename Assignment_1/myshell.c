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
 int nargs=0;char **args;char *temp2;int inpt=0,otpt=0;
 
 do
{  
 nargs =0;inpt=0;otpt=0;
 printf("myshell>");
 fgets(command,200,stdin);
 if(strcmp(command,"\n")==0)
{ continue;}
 command[strlen(command)-1]='\0';
 char is_and = command[strlen(command)-1];
 strcpy(temp,command);
 temp2=strtok(command," ");nargs++;
 while((temp2=strtok(NULL," "))!=NULL)
   nargs++;
 args = (char **)malloc(nargs * sizeof(char *));
 
 args[0] = strtok(temp," ");      
 for(i=1;i<nargs;i++)
 {
   args[i] = strtok(NULL," ");
   if(strcmp(args[i],"<")==0)
    inpt=i;
   if(strcmp(args[i],">")==0)
    otpt=i;
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
        
       }else{
        if(is_and != '&')       
         waitpid(p,&status,0);
       
      }      
       
  }											
	
}while(strcmp(args[0],"exit")!=0);

}
