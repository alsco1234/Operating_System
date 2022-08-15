#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_LEN 256

#define READ_END 0
#define WRITE_END 1

void Parent(int out);
void Child(int in);

int main()
{
	printf("Input messages and child will print it. Type \"quit\" to quit.\n");

	int fd[2] = { 0 };
	if(pipe(fd) < 0){
		fprintf(stderr, "Failed to create pipe.\n");
		return -1;
	}

	pid_t child_pid = fork();

	if(child_pid < 0){
		fprintf(stderr, "Failed to create process.\n");
return -1;

	} else if(child_pid > 0){
		close(fd[READ_END]);
		Parent(fd[WRITE_END]);
		printf("Terminating parent.\n");
		// previous code

		// updated code
		wait(NULL);

	} else {
		close(fd[WRITE_END]);
		Child(fd[READ_END]);
		printf("Terminating child.\n");
	}
	return 0;
}

void Parent(int out)
{
	char out_mesg[MAX_LEN] = "";

	while(1){
		fgets(out_mesg, MAX_LEN, stdin);
		out_mesg[strlen(out_mesg)-1] = '\0';
		if(strcmp(out_mesg, "quit")==0) break;
		write(out, out_mesg, strlen(out_mesg)+1);
	}
	close(out);
}

void Child(int in)
{
	char in_mesg[MAX_LEN] = "";

	while(1){
		if(read(in, in_mesg, MAX_LEN)==0)break;
		printf("[child] in_mesg = [%s]\n", in_mesg);
	}
	close(in);
}

