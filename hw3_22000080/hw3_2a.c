#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(){
	struct timeval start_time, end_time;
	double running_time = 0.000000;

	// Display pid and ppid using getpid() and getppid()
	printf("[parent] before fork(), pid = %ld, ppid = %ld\n", 
		(long)getpid(), (long)getppid());
	
	// Get current time using gettimeofday() (start_time)
	gettimeofday(&start_time, NULL);
	
	// Create a child by fork()
	pid_t child_pid = fork();
	
	// fork error
	if(child_pid < 0){
		fprintf(stderr, "fork failed\n");
		exit(-1);
	}
	// In child	
	else if(child_pid == 0){
		printf("[child] pid = %ld, ppid = %ld, child_pid = %d\n", 
			(long)getpid(), (long)getppid(), child_pid);
		execl("./hw3_2b", "./hw3_2b", NULL);
	}
	//In parent
	else {
		wait(NULL);
		gettimeofday(&end_time, NULL);
		running_time += end_time.tv_sec - start_time.tv_sec;
		running_time += (end_time.tv_usec - start_time.tv_usec)*0.000001;
		printf("[parent] pid = %ld, ppid = %ld, child_pid = %d, running time = %lf\n", 
			(long)getpid(), (long)getppid(), child_pid, running_time);
	}
	return 0;
}
