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

void Parent(int shm_id);
void Child(int shm_id);

int main()
{
	// TO DO: create a shared memory block to store Buffer.
	//	On failure, print an error message.
	int shm_id = shmget(IPC_PRIVATE, BUFFER_SIZE, S_IRUSR|S_IWUSR);
	if(shm_id<0){
		printf("create shared mem error\n");
		return -1;
	}

	pid_t child = fork();

	if(child < 0){
		printf("Failed to create process.\n");
		exit(-1);
	} else if(child > 0){	// parent	
		Parent(shm_id);						// Do not modify this line.

		// TO DO: Remove the shared memory block.
		shmctl(shm_id, IPC_PRIVATE, NULL);

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
	char * shared_mem = (char *) shmat(shm_id, NULL, 0);
	if(!shared_mem){
		printf("attach error!\n"); 
		return;
	}

	// TO DO: initialize the shared buffer by setting the 'loaded' field by zero.
	Buffer bf;
	bf.loaded = 0;

	printf("Type keys and child will display it. Press \"quit\" to end.\n");

	do {
		// TO DO: 
		//	Read a text line until the Enter key using fgets().
		char text[BUFFER_SIZE];
		fgets(text, BUFFER_SIZE, stdin);
		text[strlen(text)-1] = '\0';
	 	//  Wait while the buffer is loaded.
		sleep(1);
		//	Copy the input text to the buffer and set 'loaded' to TRUE.
		sprintf(shared_mem, "%s", text);
		bf.loaded = TRUE;

		//	If the input text is "quit", break the loop.
		if(strcmp(text, "quit")==0) break;

	} while(1);

	// TO DO: Detatch the shared memory block.
	shmctl(shm_id, IPC_RMID, NULL);
}

void Child(int shm_id)
{
	sleep(1); // not to access buffer before initialization


	// TO DO: attach the shared memory block in a Buffer type pointer.
	//	On failure, print an error message and terminate.
	Buffer bf;
	char * shared_mem = (char *) shmat(shm_id, NULL, 0);
	if(!shared_mem){
		printf("attach error!\n"); 
		return;
	}

	do {
		// TO DO:
		//	wait until the buffer is loaded.
		sleep(1);

		//	Retrieve the text from the buffer and set the 'loaded' field to FALSE.
		//		(Copy the text in the buffer to a local variable)
		Buffer bf;
		sprintf(bf.text, "%s", shared_mem);
		bf.loaded = FALSE;

		//	If the retrieved text is "quit", break the loop.
		if(strcmp(bf.text, "quit")==0) break;
		//	Display the retrieved text.
		//		Display a prefix "[child]" before the text.
		printf("[child] in_mesg = [%s]\n", bf.text);
		
		if(bf.loaded) strcpy(bf.text, "");
	} while(1);

	// TO DO: Detatch the shared memory block.
	shmctl(shm_id, IPC_RMID, NULL);
}
