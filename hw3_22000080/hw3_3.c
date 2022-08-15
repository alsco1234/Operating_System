#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND 512
#define MAX_ARG 256

void ParseCommand(char *command, int*argc, char *argv[]);

int main(){
	char buf[MAX_COMMAND] = {0,};
	char command[MAX_COMMAND];
	int argc = 0;
	char *argv[MAX_ARG] = {NULL};

	//Display a welcome message 
	printf("Welcome to my_shell!\n");

	//Repeat
	while(1){
		//Display a prompt including current directory
		getcwd(buf, MAX_COMMAND);
		printf("%s> ", buf);
		//Read a command line from the user
		fgets(command, MAX_COMMAND, stdin);
		command[strlen(command) -1] = 0;
		//Parse the command line into arguments using ParseCommand()
		ParseCommand(command, &argc, argv);
		//If the command is
		if(command==NULL) continue;
		else if(strcmp(command,"exit")==0)
			break;
		else if(strcmp(argv[0],"cd")==0)
			chdir(argv[1]);
		else{
			pid_t child_pid = fork();
			if(child_pid<0){
				fprintf(stderr, "fork failed\n");
				exit(-1);
			}
			else if(child_pid ==0){
				if(execvp(argv[0], argv) == -1){
					printf("Cannot execute command\n");
					kill(child_pid, SIGINT);
				}
			}
			else {
				if(strcmp(argv[argc-1],"&")==0)
					argv[argc-1] = "/0";
				else
					wait(NULL);
			}
		}
	}
	// Display a good-bye message
	printf("Bye!\n");
	return 0;
}

void ParseCommand(char *command, int *argc, char *argv[]){
	command[strlen(command)] = ' ';
	*argc = 0;
	for(int i=0; i<MAX_ARG; i++)
		argv[i] = NULL;
	int i=0;

	while(1){
		if(command[i] == '\0'){
			argv[*argc] = NULL;
			break;
		}
		else if(command[i] == ' ' && argv[*argc]!=NULL){
			command[i] = '\0';
			*argc+=1;
		}
		else if(argv[*argc]==NULL && command[i] != ' '){
			argv[*argc] = &command[i];
		}
		i+=1;
	}
}
