#include <stdio.h>
#include <string.h>
#include "libmsg.h"

int main()
{
  char user_input[100];
  char user_input2[100];
  int ret;
  struct str_message *buff;
  buff = (struct str_message *)malloc(sizeof(struct str_message));
  printf("Welcome to Messaging Program\n");
  printf("Enter register <registration name> to register yourself\n");
  scanf("%s",user_input);
  if(strcmp(user_input,"register")==0)
  {
    scanf("%s",user_input);
    while(strlen(user_input)>10)
     {
       printf("Registration Name is Max 10 characters");
     }
    ret = registr(user_input);
    if(ret == 1)
    {
    	printf("Registration Succesfull\n");
    	printf("You can know use the following commands : \n");
    	printf("1. send <recipient name> <message>\n");
    	printf("2. listmessages\n");
    	printf("3. read\n");
    	printf("4. deregister\n");
    	printf("5. quit\n");
    	while(1)
    	{
    	  scanf("%s",user_input);
    	  if(strcmp(user_input,"quit")==0)
    	  {
    	    exit(0);
    	  
    	  }
    	  else if(strcmp(user_input,"listmessages")==0)
    	  {
    	    ret = listmessages();
    	    if(ret ==0)
    	    printf("No messages to read\n");
    	  }
    	  else if(strcmp(user_input,"deregister")==0)
    	  {
    	    ret = deregister();
    	  }
    	  else if(strcmp(user_input,"read")==0)
    	  {
    	     ret = readmessage(buff);
    	     if(ret==1)
    	     {
    	     	printf("Sender %s\n",buff->sender);
    	     	printf("Message :\n %s\n",buff->message);
    	     	printf("Time : %d\n",buff->time);
    	     }
    	     else
    	     printf("No messages to read\n");
    	  }
    	  else if(strcmp(user_input,"send")==0)
    	  {
    	    scanf("%s",user_input);
    	    scanf("%s",user_input2);
    	    ret = sendmessage(user_input,user_input2);
    	    if(ret ==1)
    	     printf("Message Sent to %s\n",user_input);
    	    else
    	     printf("Sending Failed\n");
    	  }
    	}
    }
  }
  else
  printf("Invalid Command");
}
