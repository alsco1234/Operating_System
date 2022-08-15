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
	printf("Welcome to my_shell!\n");
	
	while(1){
		getcwd(buf, MAX_COMMAND);
		
		// previous code
		/*
		printf("%s> ", buf);
		*/

		// updated code
		printf("my_shell %s> ", buf);

		fgets(command, MAX_COMMAND, stdin);
		command[strlen(command) -1] = 0;
		ParseCommand(command, &argc, argv);
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
