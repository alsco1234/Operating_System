#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LEN 256

#define READ_END 0
#define WRITE_END 1


// DO NOT declare global variables

void Parent(int out);
void Child(int in);

int main()
// TO DO: The main function contains bugs. Find and fix ALL bugs.
// 		(Changing correct statements will be penalized.)
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
		Parent(fd[WRITE_END]);// DO NOT modify this line.
		printf("Terminating parent.\n");	// This message MUST be displayed.
	} else {
		close(fd[WRITE_END]);
		Child(fd[READ_END]);				// DO NOT modify this line.
		printf("Terminating child.\n");		// This message MUST be displayed. (IMPORTANT!)
	}
	return 0;
}

void Parent(int out)
// TO DO: Complete this function following the instructions.
{
	char out_mesg[MAX_LEN] = "";

	while(1){
		// TO DO:
		//	read a text line until the enter key using fgets().
		fgets(out_mesg, MAX_LEN, stdin);
		out_mesg[strlen(out_mesg)-1] = '\0';
		printf("test : out_mesg is %s\n", out_mesg);
		//	if the input text is "quit", break the loop.
		if(strcmp(out_mesg, "quit")==0) break;
		//	write the text into the pipe.
		write(out, out_mesg, strlen(out_mesg)+1);
		//	DO NOT display any message from the parent process.
	}
	// TO DO: Close the pipe.
	close(out);
}

void Child(int in)
// TO DO: complete this function following the instructions
{
	char in_mesg[MAX_LEN] = "";

	while(1){
		// TO DO:
		// 	Read a message from the pipe.
		if(read(in, in_mesg, MAX_LEN)==0)break;
		//		On failure (the pipe was broken), break the loop.
		//		Hint) If all ends of a pipe were closed, read() from the pipe returns zero
		//  Print the message read from the pipe.
		//		Put a prefix "[child]" to indicate its a message from the child.
		printf("[child] in_mesg = [%s]\n", in_mesg);
	}

	// TO DO: Close the pipe.
	close(in);
}

