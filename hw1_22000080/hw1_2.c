#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

int ReadTextLine(int fd, char str[], int max_len);

char buffer[BUFFER_SIZE];
int buffer_size = 0;
int buffer_pos = 0;

int main(){
	int corenum = 0; //# of CPU cores
	char modelname[6][80] = {0}; //CPU model name
	int totalmem =0 ; //Total memory
	float loadavg[3] = {0.0}; //Average workload for prevous mins

	int fd1 = 0;
	fd1 = open("/proc/cpuinfo", O_RDONLY); //for # of CPU cores, CPU model name
	if(fd1<0) printf("fail to open cpuinfo file\n"); //file open error
	for(int i=0; i<15; i++){ 
		if(ReadTextLine(fd1, buffer, 128)==EOF)break; //EOF check
		if(strstr(buffer, "cpu cores")) //if cpu core info line
			sscanf(buffer, "cpu cores\t: %d", &corenum); //get num
		else if(strstr(buffer, "model name")) //if model name info line
			sscanf(buffer, "model name\t: %s %s %s %s %s %s",  //get name
			modelname[0], modelname[1], modelname[2], modelname[3], modelname[4], modelname[5]);
	}
	close(fd1); //cpuingo file close
	
	int fd2 = 0;
	fd2 = open("/proc/meminfo", O_RDONLY); //for total mem
	if(fd2<0) printf("Failed to open meminfo file\n");
	ReadTextLine(fd2, buffer, 128); //first line
	sscanf(buffer, "MemTotal:\t%d kb", &totalmem); //get totalmem
	close(fd2); //meminfo file close

	int fd3 = 0;
	fd3 = open("/proc/loadavg", O_RDONLY); //for loadavg
	if(fd3<0) printf("Failed to open loadagv file\n");
	ReadTextLine(fd3, buffer, 128); //first line
	sscanf(buffer, "%f %f %f", &loadavg[0], &loadavg[1], &loadavg[2]); //get loadavgs
	close(fd3); //loadavg file close
	
	//print
	printf("# of processor cores = %d\n", corenum);
	printf("CPU model = %s %s %s %s %s %s\n", modelname[0], modelname[1], modelname[2], modelname[3], modelname[4], modelname[5]);
	printf("MemTotal = %d\n", totalmem);
	printf("loadavg1 = %f, loadavg5 = %f, loadavg15 = %f\n",
		loadavg[0], loadavg[1], loadavg[2]);

	return 0;
}

int ReadTextLine(int fd, char str[], int max_len){
	int i = 0;
	int j = 0;
	int ret = 0;

	if(lseek(fd, 0, SEEK_CUR) == 0)
		buffer_pos = buffer_size = 0;

	while(j < max_len - 1){
		if(buffer_pos == buffer_size){
			buffer[0] = 0;
			buffer_size = read(fd, buffer, BUFFER_SIZE);
			buffer_pos = 0;
		}
	
		if(buffer_size == 0){
			if(j == 0)
				ret = EOF;
			break;
		}

		while(j < max_len - 2 && buffer_pos < buffer_size){
			str[j++] = buffer[buffer_pos++];
			if(str[j-1] == '\n' || str[j-1] == 10){
				j--;
				max_len = j;
				break;
			}
		}
	}

	str[j] = 0;

	return ret;
}
