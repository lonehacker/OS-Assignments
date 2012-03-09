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
 
 char cmd[100];char temp[200];
 char command[200];
 char tmp;int i=0;char out[100];int status;
 struct dirent *myfile;
 int mkdir_stat;
 struct stat sb;
 DIR *dp = NULL;
 printf("myshell>");
 while((tmp = getchar())!=EOF)
 {
  
  
  if(tmp == '\n')
  { 
    printf("myshell>");
  }
  else{
   cmd[0] = tmp;
   cmd[1] = '\0';
   scanf("%s",temp);
   strcat(cmd,temp);
    if(strcmp(cmd,"exit")==0)
    {
      exit(0);
    }else if (strcmp(cmd,"hello")==0)
    {
      printf("%s",cmd); 
    }
   else if(strcmp(cmd,"pwd")==0)
    {
     if(getcwd(out,100)){
	printf("%s\n",out);
     }
    }				
  else if(strcmp(cmd,"ls")==0)
   {
    scanf("%s",temp);
    
 
  if(getcwd(out,100))
   {
    dp = opendir(out);
    while(myfile = readdir(dp))
        if(strcmp(myfile->d_name,".")!=0 && strcmp(myfile->d_name,"..")!=0)
	if(strcmp(temp,"-l")==0)
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
        else
         printf("%s ",myfile->d_name);
    }
    printf("\n");
   }

  else if(strcmp(cmd,"cd")==0)
  {
	scanf("%s",temp);
        if(chdir(temp)==-1){
	printf("Error occured during execution of command\n");
	}
  }
  else if(strcmp(cmd,"mkdir")==0)
 {	scanf("%s",temp);
        mkdir_stat = umask(0);
        if((mkdir_stat = mkdir(temp,0777))!= 0)
        
         printf("mkdir failed");
  }
  else if(strcmp(cmd,"rmdir")==0)
  {	
        scanf("%s",temp);
        if((mkdir_stat = rmdir(temp))!= 0)
        
         printf("rmdir failed");
  }
  else{
       fgets(command,200,stdin);
       command[strlen(command)-1]='\0';
       //int p = fork();
       //if(p==0)
       //{
         int nargs=0;char **args;int i;char *temp2;
         char *env = getenv("PATH");
         
         strcat(env,":.");
	 setenv("PATH",env,1);
         strcpy(temp,command);
         //printf("%s\n",command);
         temp2=strtok(command," ");nargs++;
         while((temp2=strtok(NULL," "))!=NULL)
           nargs++;
        args = (char **)malloc(sizeof(char *)*nargs);
        args[0] = strtok(temp," ");
        for(i=1;i<nargs;i++)
          args[i] = strtok(NULL," ");
        
        
        exit(0);
       //}else{
          //waitpid(i,&status,0);
         // printf("Command not found\n");
       //}      
       
      }											
	




   }
   
 }

}
