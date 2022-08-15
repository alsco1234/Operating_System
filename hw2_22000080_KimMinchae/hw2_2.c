#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define MAX_PATH 256

int main(){
	// Open directory using opendir
	DIR *dir_ptr = NULL;
	struct dirent* entry = NULL;
	struct stat buf;
	char *cur_path = NULL;
	char strBuffer[MAX_PATH] = {0,};

	cur_path = getcwd(strBuffer, MAX_PATH);

	if((dir_ptr=opendir(cur_path))==NULL)
		printf("open error\n");
	
	// Read for all directory entries
	while((entry = readdir(dir_ptr)) != NULL){
		lstat(entry->d_name, &buf);

		if(S_ISDIR(buf.st_mode))
			printf("[dir] %s\n", entry->d_name);
		else if (S_ISREG(buf.st_mode)){
			struct tm* t = localtime(&buf.st_mtime);
			printf("[reg] %s  uid=%d,  gid=%d,  %ldbytes, mtime=",
				entry->d_name, buf.st_uid, buf.st_gid, buf.st_size);
			printf("%d:%02d:%02d %02d:%02d:%02d\n", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		}
	}

	// Close directory using closedir
	closedir(dir_ptr);
	return 0;
}
