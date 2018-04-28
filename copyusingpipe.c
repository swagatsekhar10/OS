#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<sys/stat.h>
#define BUFFER_SIZE 50
int main( int a, char* ar[] ) 
{
	int fdone[2];
	pid_t cid;
  	char readBuff[BUFFER_SIZE];
	pipe(fdone);
	if( a != 3 ) 
		{
	    	printf("ERROR:Exactly 2 parameters needed.\n");
    		exit(1);
    	}
	int fileOpen = open(ar[1], 0);
	int targetFile = open(ar[2], O_RDWR|O_CREAT|O_APPEND, 0666);
	if (fileOpen == -1 || targetFile == -1)
    	{
			printf("ERROR:Failed to open the file\n");
			exit(1);
    	}
	cid = fork();
    if (cid == 0) 
		{
        	close(fdone[1]);
        	while (read(fdone[0], readBuff, sizeof(readBuff)) > 0) 
				{
            		write(targetFile, readBuff, strlen(readBuff) - 1);
        		}
        	close(fdone[0]);
        	close(targetFile);
    	}
    else 
		{
        	close(fdone[0]);
        	while (read(fileOpen, readBuff, sizeof(readBuff)) > 0) 
				{
            		write(fdone[1], readBuff, sizeof(readBuff));
            		memset(readBuff, 0, BUFFER_SIZE);
        		}
        	close(fdone[1]);
        	close(fileOpen);
        	wait(NULL);    
		}
}

