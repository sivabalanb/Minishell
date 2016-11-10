/* Author(s): Sivabalan Balasubramanian
 *
 * Date: Spring 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include<limits.h>
#define MAXLINE 80
#define MAXARGS 20
#define MAX_PATH_LENGTH 50

/* function prototypes */
void process_input(int argc, char **argv);
int parseline(char *cmdline, char **argv);
void handle_redir(int argc, char *argv[]);

/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */
int main(void)
{
    char cmdline[MAXLINE];
    char *argv[MAXARGS];
    int argc;
    int status;
    pid_t pid;

    while (1) {
	printf("csc60mshell> ");
	fgets(cmdline, MAXLINE, stdin);

	argc=parseline( cmdline, argv);
	/* Handle build-in command: exit, pwd, or cd  */
	if(argc==0)
        {
	continue;
	}
        /* Handling EXIT */
	else if( (strcmp(cmdline, "exit")) == 0 )
	{
          _exit(EXIT_SUCCESS);
        }
	/* Handing PWD */
	else if (strcmp(cmdline, "cd")==0)
	{
        char *temp;
	char tempbuf[256];
      		if(argv[1])
     		{  
		chdir(argv[1]); 
		}
     		 else
		{
        	temp = getenv("HOME");
        	chdir(temp);
      		}
     	 getcwd(tempbuf,256);
     	 setenv("PWD", tempbuf,1);
	}
	/* Handling PWD */
	else if (strcmp(cmdline, "pwd")==0)
	{
	char* cwd;
   	char buff[PATH_MAX + 1];
	cwd = getcwd( buff, PATH_MAX + 1 );
    		if( cwd != NULL ) 
       	 	{ 
		printf( " %s\n", cwd );
		}
	}
	/* Else, fork off a process */
	else
	{
	pid = fork();
        switch(pid)
	{
	    case -1:
	printf("\n Case -1 in main loop");
		perror("\nShell Program fork error");
	        exit(1);
	    case 0:
		process_input(argc, argv);
		break;
	    default:
		/* I am parent process */
		if (wait(&status) == -1)
		    perror("Shell Program error");
		else
		break;
	} /* end of the switch */	
	} /* end of else*/  
 } /* end of the while */
} /* end of main */
/* ----------------------------------------------------------------- */
/*                  parseline                                        */
/* ----------------------------------------------------------------- */
/* parse input line into argc/argv format */

int parseline(char *cmdline, char **argv)
{
    int count = 0;
    char *separator = " \n\t";
 
    argv[count] = strtok(cmdline, separator);
    while ((argv[count] != NULL) && (count+1 < MAXARGS)) {
	argv[++count] = strtok((char *) 0, separator);
    }		
    return count;
}
/* ----------------------------------------------------------------- */
/*                  process_input                                    */
/* ----------------------------------------------------------------- */
void process_input(int argc, char **argv) 
{
if((strcmp(argv[0],">")==0) || (strcmp(argv[0],"<")==0))
        {
                fprintf(stderr,"\nError\n");
		_exit(-1);		
        }      
handle_redir(argc,argv);
int i;
char *cmd[argc+1];
        for( i=0;i<=argc;i++)
        {
        cmd[i]=argv[i];
        
         }
        cmd[argc+1]=NULL;
        execvp(cmd[0],cmd);
}
/* ----------------------------------------------------------------- */
/*			handle_redir				     */
/* ----------------------------------------------------------------- */
void handle_redir(int argc, char *argv[])
{
	if(argc>3)
	{
	if(((strcmp(argv[1],">")==0) || (strcmp(argv[1],"<")==0)) && ((strcmp(argv[3],">")==0) || (strcmp(argv[3],"<")==0)))
	{
                fprintf(stderr,"\nError\n");
		_exit(-1);
        }
	}
	int i;
	for(i=0;i<argc;i++)
	{
	if(strcmp(argv[i],">")==0)
	{
	int fd;
	if(argv[2] == NULL)
	{ 
	fprintf(stderr,"\nError\n");
	_exit(-1);
	 }	
	
 	int index;
	fd = open(argv[i+1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
	dup2(fd, 1);
	close(fd);
	argc--;
	printf("\nargc:%d",argc);
	}
	
	
	if(strcmp(argv[i],"<")==0)
	{
	int fd1 ;
	if(argv[2] == NULL)
        { 
	fprintf(stderr,"\nError\n"); 
	_exit(-1);
	}
	fd1= open(argv[i+1], O_RDONLY, 0600);  
	dup2(fd1, 0);
	close(fd1);
	}
	}
}
/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */
