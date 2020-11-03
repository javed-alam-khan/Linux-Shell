#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include<sys/wait.h> 
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#define BUF 1000


int main()
{
    int argc;              
    char* argv[100];        
    char cinput[BUF];         
    //char path[] = "/bin/";
    char path[] = "";   
    char fpath[25];                   

  while(1)
  {
    printf("shell>> ");                    

    if(!fgets(cinput, BUF, stdin))
    break;
    size_t size = strlen(cinput);
    if (cinput[size-1] == '\n')
    cinput[size-1] = '\0';                              
   

    if(strcmp(cinput, "exit\0")==0)    
    {    
        break;
    }
    
    int i=0;
    char *token;                 
    token = strtok(cinput," ");
    while(token!=NULL)
    {
        argv[i]=token;      
        token = strtok(NULL," ");
        i++;
    }
    argv[i]=NULL; 
    
    if(strcmp(argv[0],"cd")==0)
        chdir(argv[1]);                    
    if(strcmp(argv[0],"exit")==0)
           break; 
    argc=i;                           
    
    strcpy(fpath, path);           
    strcat(fpath, argv[0]); 


   int flag = 0;
   i=0;
   while(argv[i] != NULL)
   {
        int pid;
        int fd;
        if(strcmp(argv[i],">")==0)
        {
                 argv[i]=NULL;
                 pid = fork();
                 if(pid!=0)
                 {
                   wait(NULL);
                   fd = open(argv[i+1],O_WRONLY|O_CREAT);
	           chmod(argv[i+1],S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);

                   dup2(fd,1);
                   close(fd);
                   flag = 1;
                 }
                 else
                 {
                   execvp(argv[0],argv);
                   exit(0);
                 }

        }
	else if(strcmp(argv[i],">>")==0)
        {
                 argv[i]=NULL;
                 pid = fork();
                 if(pid!=0)
                 {
                   wait(NULL);
                   fd = open(argv[i+1],O_WRONLY|O_APPEND|O_CREAT);
                   chmod(argv[i+1],S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);

                   dup2(fd,1);
                   close(fd);
                   flag = 1;
                 }
                 else
                 {
                    execvp(argv[0],argv);
                    exit(0);
                 }
	   }	 
          i++;
        }



    if(flag == 1)
        break;







    size_t pid= fork();              
    if(pid==0)
    {               
        execvp(fpath,argv);
        
    }
    else
    {                   
        wait(NULL);
    }
   
  }
} 
