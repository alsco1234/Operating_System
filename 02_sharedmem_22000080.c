/*

	This program reads messages from the user on a process and display it on the other process.
	The parent and child processes communicate through System-V shared memory.

	Parent)
		- Create a shared memory block using shmget().
		- Create a child process.
		- Attach the shared memory block.
		- Initialize the buffer
		- Repeat until the user types "quit".
			Read a text line from the user and put the input text into the shared buffer.
		- Detatch the shared memory block.

	Child)
		- Wait for 1 second.
		- Attach the shared memory block.
		- Repeat until the received message is "quit".
			Retrieve the message from the buffer and display the retrieved message.
		- Detatch the shared memory block


	Complete the program.


	$ ./a.out
	Type keys and child will display it. Press "quit" to end.
	welcome						// user's input
	==> [child] retrieved message = welcome
	nice to see you				// user's input
	==> [child] retrieved message = nice to see you
	hello						// user's input
	==> [child] retrieved message = hello
	God bless you!				// user's input
	==> [child] retrieved message = God bless you!
	quit						// user's input
	Terminating parent.			// The last two lines can be exchanged
	Terminating child.
	
	
	Sometimes, unnecessary child processes remains while debugging.
	Hint) Checking processes whose name is 'a.out':
		$ ps -aux | grep a.out
	Hint) Killing all processes named 'a.out':
		$ killall a.out

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

#define BUFFER_SIZE 256

typedef struct {
	int loaded;
	char text[BUFFER_SIZE];
} Buffer;

void Parent(int shm_fd);
void Child(int shm_fd);

int main()
{
	// TO DO: create a shared memory block to store Buffer.
	//	On failure, print an error message.
	if(shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR) == -1){
	printf("create sharedmem error\n");
	exit(-1);
	}


	pid_t child = fork();

	if(child < 0){
		printf("Failed to create process.\n");
		exit(-1);
	} else if(child > 0){	// parent	
		Parent(shm_id);						// Do not modify this line.

		// TO DO: Remove the shared memory block.
		unlink(shmpath);

		printf("Terminating parent.\n");	// This message MUST be displayed.
	} else {				// child
		Child(shm_id);						// Do not modify this line.

		printf("Terminating child.\n");		// This message MUST be displayed.	 (IMPORTANT!)
	}

	return 0;
}

void Parent(int shm_id)
{
	// TO DO: Attach the shared memory block and receive the address in a Buffer type pointer.
	//	On failure, print an error message.
	if(shm_open(shm_id, , ) < 0){
		printf("shared mem open error!\n");
		return -1;
	}


	// TO DO: initialize the shared buffer by setting the 'loaded' field by zero.

	printf("Type keys and child will display it. Press \"quit\" to end.\n");

	do {
		// TO DO: 
		//	Read a text line until the Enter key using fgets().
		fgets(text, MAX_LEN, buffer);
		// 	Wait while the buffer is loaded.
		wait(NULL);
		//	Copy the input text to the buffer and set 'loaded' to TRUE.
		//	If the input text is "quit", break the loop.
		if(strcmp(text, "quit")==0) break;


	} while(1);

	// TO DO: Detatch the shared memory block.
	shm_inlink(shm_id);
}

void Child(int shm_id)
{
	sleep(1);		// not to access buffer before initialization


	// TO DO: attach the shared memory block in a Buffer type pointer.
	//	On failure, print an error message and terminate.
	if(mmap(shm_id, MAX_LEN, , ,)<0){
		printf("mmap error!\n");
		truncate(shm_id);
	}

	do {
		// TO DO:
		//	wait until the buffer is loaded.
		wait(NULL);
		//	Retrieve the text from the buffer and set the 'loaded' field to FALSE.
		//		(Copy the text in the buffer to a local variable)
		int out = Buffer.text;
		Buffer.loaded = FALSE;
		//	If the retrieved text is "quit", break the loop.
		if(strcmp(text, "quit")==0) break;
		//	Display the retrieved text.
		//		Display a prefix "[child]" before the text.
		printf("[child] %s\n", text);



	} while(1);

	// TO DO: Detatch the shared memory block.
	unlink(shm_id);
}
