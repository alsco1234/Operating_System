#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH 256
#define MAX_RESULT 100

void FindLargestFile(char *start_dir, char *largest, long *size);

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Usage: %s <start_dir>\n", argv[0]);
		return 0;
	}

	char *start_dir = argv[1];
	char largest[MAX_PATH] = "";
	long size = 0L;
	
	FindLargestFile(start_dir, largest, &size);

	if(size > 0)
		printf("largest file = %s, size = %ld\n", largest, size);
	else
		printf("no file with size > 0.\n");
	
	return 0;
}

void FindLargestFile(char *start_dir, char *largest, long *size){
	// Open directory using opendir
	DIR *dir_ptr = NULL;
	struct dirent* entry = NULL;
	struct stat buf;
	
	if((dir_ptr = opendir(start_dir))==NULL){
		printf("open %s dir error\n", start_dir);
		exit(1);
	}	
	// Repeat for all directory entries
	while((entry = readdir(dir_ptr))!=NULL){
		if(lstat(entry->d_name, &buf) < 0){
			printf("%s lstat error\n", entry->d_name);
			exit(1);
		}

		//if dir
		if(S_ISDIR(buf.st_mode)){
			if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
				FindLargestFile(strcat(strcat(start_dir,"/"),entry->d_name), largest, size);
		}
		//if file
		else if(S_ISREG(buf.st_mode)){
			if(buf.st_size > *size){
				strcpy(largest, entry->d_name);
				*size = buf.st_size;
			}
		}
	}

	// Close directory using closedir
	closedir(dir_ptr);
}
