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

#define FILENAME "hw4_3_22000080.shm"
#define BUFFER_SIZE 128
#define TRUE 1
#define FALSE 0

typedef struct {
	char buffer[BUFFER_SIZE];
	int in, out;
} KeyBuffer;

void InitBuffer(KeyBuffer *buf);
int IsEmpty(KeyBuffer *buf);
int IsFull(KeyBuffer *buf);
int GetKeyCount(KeyBuffer *buf);

void InsertKey(KeyBuffer *buf, char key);
char DeleteKey(KeyBuffer *buf);

void Producer(int shm_fd);
void Consumer(int shm_fd);

int main(void){
	//Create a shared file FILENAME using shm_open
	int shm_fd = shm_open(FILENAME, O_CREAT|O_RDWR, 0666);

	//Set the size of the shared file to sizeof(KeyBuffer) using ftruncate
	ftruncate(shm_fd, sizeof(KeyBuffer));

	//Create a child process
	pid_t child = fork();
	
	if(child<0){
		printf("Failed to create child process.\n");
		return -1;
	}
	else if(child>0){
	//On parent process, call Producer
		Producer(shm_fd);
		printf("Terminating producer\n");
	}
	else{
	//On child process, call Consumer
		Consumer(shm_fd);
		printf("Terminating comsumer\n");
	}

	return 0;
}

void InitBuffer(KeyBuffer *buf){
	//Set in and out to zero
	buf->in = 0;
	buf->out = 0;
}

int IsEmpty(KeyBuffer *buf){
	//return 1 if buf is empty, otherwise, 0
	if(buf->in == buf->out) return 1;
	else return 0;
}

int IsFull(KeyBuffer *buf){
	//return 1 if buf is full, otherwise, 0
	if((buf->in +1) % BUFFER_SIZE == buf->out) return 1;
	else return 0;
}

int GetKeyCount(KeyBuffer *buf){
	//return the number of keys in the buffer
	//computed the number of keys from in and out
	return (((buf->in - buf->out) + BUFFER_SIZE) % BUFFER_SIZE);
}

void InsertKey(KeyBuffer *buf, char key){
	//insert a key into buf
	buf->buffer[buf->in] = key;
	buf->in = (buf->in + 1) % BUFFER_SIZE;
}

char DeleteKey(KeyBuffer *buf){
	//delete a key from buf and return the delete key
	char item = buf->buffer[buf->out];
	buf->out = (buf->out + 1) % BUFFER_SIZE;
	return item;
}

void Producer(int shm_fd){
	//Map shm_fd to its memory space using mmap
		//use PROT_WRITE | PROT_READ
	KeyBuffer *buf = mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

	//Initial buf by InitBuffer()
	InitBuffer(buf);
	
	char key;
	printf("Type keys. Press ESC to end\n");
	//Repeat
	while(1){
		//Read a key from the user
		scanf(" %c", &key);
		//If the key is ESC(27), insert '\0' into buf twice (If buf is full, wait)
		while(IsFull(buf));
		if(key==27){
			InsertKey(buf, key);
			InsertKey(buf, '\0');
			InsertKey(buf, '\0');
			break;
		}
		//Otherwise, insert the key into buf
		else{
			InsertKey(buf, key);
		}
		//Display the key and the state of buf
		printf("[Producer] inserting key = %c (IsEmpty = %d, IsFull = %d, KeyCount = %d, in = %d, out = %d)\n",
		key, IsEmpty(buf), IsFull(buf), GetKeyCount(buf), buf->in, buf->out);
	}
	wait(NULL);
}

void Consumer(int shm_fd){
	//Map shm_fd to its memory space using mmap()
		//use PROT_WRITE | PROT_READ	
	KeyBuffer *buf = mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

	//Repeat
	while(1){
		//If buf contains less then three keys, wait
		while(GetKeyCount(buf) < 3);
		//Retrieve a key from buf using DeleteKey()
		char key = DeleteKey(buf);
		//If the retieved key is ESC(27), break the loop
		if(key==27) break;
		//Otherwise, display the retrieved key and the state of buf
		printf("==> [Consumer] retrieved key = %c (IsEmpty = %d, IsFull = %d, KeyCount = %d, in = %d, out = %d)\n",
		key, IsEmpty(buf), IsFull(buf), GetKeyCount(buf), buf->in, buf->out);
	}

	//Unlink the shared file
	shm_unlink(FILENAME);
}

