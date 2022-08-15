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

#define SIZE 1024
#define FILENAME "hw4_2_22000080.shm"

typedef struct{
	char name[32];
	char id[32];
	char major[32];
} Student;

int main()
{	
	//PARENT
	//Create a shared file FILENAME using shm_open()
	int shm_fd = shm_open(FILENAME, O_CREAT|O_RDWR, 0666);

	//Set the file size to SIZE * sizeof(Student) using ftruncate()
	ftruncate(shm_fd, SIZE*sizeof(Student));

	//Create a child process
		//on fail, display an error message and quit.
	pid_t child = fork();
	if(child<0){
		printf("Failed to create chlid process.\n");
		return -1;
	}
	else if(child>0){
		//Map the shared file to the memory space using PROT_WRITE
			//take it using a Student pointer variable
		Student *s = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
		//File s[0], .. s[3] with the following information
			//Assumg more student can be added in the future
		//To indicate the end of array, fill s[n].id with empty string "", whr  n is the num of st
		char namelist[4][32] = {"peter Parker", "Natasha Romanoff", "Clark Kent", "Diana Pronce"};
		char idlist[4][32] = {"2200001", "2200002", "2200003", "2200004"};
		char majorlist[4][32] = {"Computer Science", "Elctric Engineering", "GLS", "CSEE"};
		int i=0;
		while(1){
			if(i == sizeof(idlist)/(sizeof(char)*32)){
				strcpy( ((Student *)s + i)->id, "");
				break;
			}
			strcpy( ((Student *)s + i)->name , namelist[i]);
			strcpy( ((Student *)s + i)->id , idlist[i]);
			strcpy( ((Student *)s + i)->major , idlist[i]); 
			i++;
		}
		wait(NULL);
	}
	//DO NOT DISPLAY ANY MESSAGE FROM PARRENT PROCESS
	
	//Child
	if(child==0){
		//Map the shared file to the memory space using PROT_READ
			//take it using a Student pointer variable (similar to the parent)
		Student *s = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

		//Dispaly s[i]'s until s[i].id is an empty string(i>=0)
			//s is the array of Student in the shared memory block
		int i=0;
		while(1){
			printf("name = %s, id = %s, major = %s\n", s[i].name, s[i].id, s[i].major);
			if(strcmp(s[++i].id, "")==0) break;
		}

		//Unlink the shared file using unlink()
		shm_unlink(FILENAME);
		close(shm_fd);
	}

	return 0;
}

