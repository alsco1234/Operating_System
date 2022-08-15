/*

	This program reads and displays messages using multiple processes.
		The parent reads messages from the user and sends it to the child through a pipe.
		The child receives the message from the parent and displays it.

	If the user types "quit", the parent breaks the loop and closes the pipe.
	If the child fails to read from the pipe (pipe was broken), breaks the loop and closes the pipe.

	Complete the program following the instructions.


	$ ./a.out
	Input messages and child will print it. Type "quit" to quit.
	hello						// user's input
	[child] in_mesg = [hello]
	nice to see you				// user's input
	[child] in_mesg = [nice to see you]
	welcome to this class		// user's input
	[child] in_mesg = [welcome to this class]
	quit						// user's input
	Terminating parent.
	Terminating child.


	$ ./a.out
	Input messages and child will print it. Type "quit" to quit.
	abcde						// user's input
	[child] in_mesg = [abcde]
	12345						// user's input
	[child] in_mesg = [12345]
								// user typed just enter
	[child] in_mesg = []
	ASDF						// user's input
	[child] in_mesg = [ASDF]
	quit						// user's input
	Terminating parent.
	Terminating child.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	pid_t child_pid = fork();

	int fd[2] = { 0 };
	if(pipe(fd) < 0){
		fprintf(stderr, "Failed to create pipe.\n");
		return -1;
	}

	if(child_pid < 0){
		fprintf(stderr, "Failed to create process.\n");
		return -1;

	} else if(child_pid > 0){
		Parent(fd[WRITE_END]);				// DO NOT modify this line.
		printf("Terminating parent.\n");	// This message MUST be displayed.

	} else {
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
		fgets(out_mesg, MAX_LEN+1, out);
		//	if the input text is "quit", break the loop.
		if(strcmp(out_mesg, "quit")==0) break;
		//	write the text into the pipe.
		for(int i=0; i<strlen(out_mesg); i++){
			write(fd[i], out_mesg[i], strlen(out_mesg[1]));
		}
		//	DO NOT display any message from the parent process.
		wait(NULL);
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
		read(in_mesg, in, 1);
		//		On failure (the pipe was broken), break the loop.
		if(pipe(in) == -1){
			printf("pipe was broken\n");
			break;
		}
		//		Hint) If all ends of a pipe were closed, read() from the pipe returns zero
		//  Print the message read from the pipe.
		printf("[child] %s\n", in_mesg);
		//		Put a prefix "[child]" to indicate its a message from the child.
	}

	// TO DO: Close the pipe.
	close(in);
}

