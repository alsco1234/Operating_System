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
	DIR *dir_ptr = NULL;
	struct dirent* entry = NULL;
	struct stat buf;
	//previous code
	/*
	*/

	//updated code
	//start
	char *cur_path = NULL;
	char strBuffer[MAX_PATH] = {0,};

	if(strcmp(start_dir,".")==0){
		cur_path = getcwd(strBuffer, MAX_PATH);
		start_dir = cur_path;
	}
	else if(strcmp(start_dir,"..")==0){
		cur_path = getcwd(strBuffer, MAX_PATH);
		strcat(cur_path, "/..");
		start_dir = cur_path;
	}
	//end

	if((dir_ptr = opendir(start_dir))==NULL){
		printf("open %s dir error\n", start_dir);
		exit(1);
	}

	while((entry = readdir(dir_ptr))!=NULL){
		//previous code
		/*
		if(lstat(start_dir, &buf) < 0){
		*/

		// updated code
		//start
		char entrypath[MAX_PATH] = "";
		strcat(entrypath, start_dir);
		strcat(entrypath,"/");
		strcat(entrypath,entry->d_name);
		if(lstat(entrypath, &buf) < 0){
		//end

			printf("%s lstat error\n", entry->d_name);
			exit(1);
		}
		
		if(S_ISDIR(buf.st_mode)){
			if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
				FindLargestFile(entrypath, largest, size);
		}
	
		if(S_ISREG(buf.st_mode)){
			if(buf.st_size > *size){
				strcpy(largest, entry->d_name);
				*size = buf.st_size;
			}
		}
	}

	closedir(dir_ptr);
}
