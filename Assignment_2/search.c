#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>  	

#define BUFSIZE 80

main()
{
  int fd1[2],fd2[2],pid1,pid2,k,status;
  int arr[100],n,found1=-1,found2=-1;
  int i;
	char line1[BUFSIZE];
       char line2[BUFSIZE];
	printf("Enter the size of the array:  ");
	scanf("%d",&n);
	printf("Enter the elements of the array:  ");
	for(i=0;i<n;i++)
	  scanf("%d",&arr[i]);
	printf("Enter the element to be searched:  ");
	scanf("%d",&k);
	
pipe(fd1); 
pipe(fd2);

if ((pid1 = fork()) == 0) {

		close(fd1[0]); 
		for (i=0; i <= n/2; i++) {
		  if(arr[i]==k)
		    found1=k;
		}   
                       	sprintf(line1,"%d",found1); 
			write(fd1[1], line1, BUFSIZE);
		}
 else if ((pid2 = fork()) == 0) {

		close(fd2[0]); 
		for (i = (n/2 -1) ; i < n; i++) {
		  if(arr[i]==k)
		    found2=k;
		}      
                       	sprintf(line2,"%d",found2); 
			write(fd2[1], line2, BUFSIZE);
		}

	
	else {
	    
			waitpid(pid1, &status, 0);
			waitpid(pid2, &status, 0);
	        	close(fd1[1]);
                  	close(fd2[1]);
			read(fd1[0], line1, BUFSIZE);
			sscanf(line1,"%d",&found1); 
                       	read(fd2[0], line2, BUFSIZE);
			sscanf(line2,"%d",&found2); 
 
			if(found1==-1 && found2==-1)
			  printf("\n\nInteger not found\n\n");
			else
			  printf("\n\nInteger found\n\n");
			
		}
	
}
