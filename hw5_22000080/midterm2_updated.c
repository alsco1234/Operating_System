#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
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
	int shm_id = shmget(IPC_PRIVATE, BUFFER_SIZE, S_IRUSR|S_IWUSR);
	if(shm_id<0){
		printf("create shared mem error\n");
		return -1;
	}

	pid_t child = fork();

	if(child < 0){
		printf("Failed to create process.\n");
		exit(-1);
	} else if(child > 0){
		Parent(shm_id);
		shmctl(shm_id, IPC_PRIVATE, NULL);
		printf("Terminating parent.\n");
		// previous code

		// updated code
		wait(NULL);

	} else {
		Child(shm_id);
		printf("Terminating child.\n");
	}
	return 0;
}

void Parent(int shm_id)
{	
	// previous code
	/*
	char * shared_mem = (char *) shmget*shm_id, NULL, 0);
	if(!shared_mem){
		printf("attach error!\n");
		return;
	}
	Buffer bf;
	bf.loaded = 0;
	do{
		char text[BUFFER_SIZE];
		fgets(text, BUFFER_SIZE, stdin);
		text(strlen(text)-1 = '\0';
		sleep(1);
		sprintf(shared_mem, "%s", text);
		bf.loaded = TRUE;
		if(strcmp(text, "quit")==0) break;
	} while(1);
	*/

	// updated code
	Buffer * shared_bf = (Buffer *) shmat(shm_id, NULL, 0);
	if(!shared_bf){
		printf("attach error!\n"); 
		return;
	}

	shared_bf->loaded = 0;

	printf("Type keys and child will display it. Press \"quit\" to end.\n");
	do {
		char text[BUFFER_SIZE];
		fgets(text, BUFFER_SIZE, stdin);
		text[strlen(text)-1] = '\0';
	 	
		while(shared_bf->loaded==TRUE);

		sprintf(shared_bf->text, "%s", text);
		shared_bf->loaded = TRUE;

		if(strcmp(text, "quit")==0) break;
	} while(1);
	shmctl(shm_id, IPC_RMID, NULL);
}

void Child(int shm_id)
{
	sleep(1);
	// previous code
	/*
	Buffer bf;
	char * shared_mem = (char *) shmat(shm_id, NULL, 0);
	if(!shared_mem){
		printf("attach error!\n");
		return;
	}
	*/

	// updated code
	Buffer * shared_bf = (Buffer *) shmat(shm_id, NULL, 0);
	if(!shared_bf){
		printf("attach error!\n"); 
		return;
	}

	do {
		// previous code
		/*
		sleep(1);
		Buffer bf;
		sprintf(bf.text, "%s", shared_mem);
		bf.loaded = FALSE;
		if(strcmp(bf.text, "quit")==0) break;
		printf("[child] in_mesg = [%s]\n", bf.text);
		if(bf.loaded) strcpy(bf.text, "");
		*/

		// updated code
		while(shared_bf->loaded==FALSE);

		char text[BUFFER_SIZE];
		sprintf(text, "%s", shared_bf->text);
		shared_bf->loaded = FALSE;

		if(strcmp(text, "quit")==0) break;
		
		printf("[child] in_mesg = [%s]\n", text);

	} while(1);
	shmctl(shm_id, IPC_RMID, NULL);
}
