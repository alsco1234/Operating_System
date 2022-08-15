#include <stdio.h>
#include <string.h>

#define MAX_CMD 2048
#define MAX_ARG 256

void ParseCommand(char *command, int *argc, char *argv[]);

int main(){
	char command[MAX_CMD];
	command[0] = command[MAX_CMD-1] = 0;

	int argc = 0;
	char *argv[MAX_ARG] = {NULL};

	while(1){
		printf("$ ");
		fgets(command, MAX_CMD - 1, stdin);
		command[strlen(command)-1] = 0;

		if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0)
			break;

		ParseCommand(command, &argc, argv);

		printf("argc = %d\n", argc);
		for(int i =0; i<argc; i++)
			printf("argv[%d] = %s\n", i, argv[i]);
		printf("argv[%d] = %p\n", argc, argv[argc]);
	}

	printf("Bye!\n");
	return 0;
}

//Prevous code
/*
Void ParseCommand(char *command, int *argc, char *argv[]){
	*argc = 0;
	for(int i=0; i<MAX_ARG; i++)
		argc[i] = NULL;
	int i=0;

	while(1){
		if(command[i] == '\0'){
			*argc+=1;
			break;
		}
		else if(command[i] == ' ' && argv[*argc]!=NULL){
			command[i] = '\0';
			*argc+1;
		}
		else if(argv[*argc]==NULL){
			argv[*argc] = &command[i];
		}
		i+=1;
	}
}
*/

// Updated code
void ParseCommand(char *command, int *argc, char *argv[]){
	//Initialization
	command[strlen(command)] = ' '; //make last space
	*argc = 0;
	for(int i=0; i<MAX_ARG; i++)
		argv[i] = NULL;
	int i =0;
	
	//look one char in commandasd
	while(1){
		if(command[i] == '\0'){ //end of command
			argv[*argc] = NULL; //argc[last] = NULL
			break;
		}
		else if(command[i] == ' ' && argv[*argc]!=NULL){ //to void spaces
			command[i] = '\0';
			*argc+=1;
		}
		else if(argv[*argc]==NULL && command[i] != ' '){ //get word's first address
				argv[*argc] = &command[i];
		}
		i+=1;
	}
}
