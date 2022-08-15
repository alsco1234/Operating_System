#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(){
	// Display pid and ppid using getpid() and getppid()
	printf("[new child] pid = %ld, ppid = %ld\n", 
		(long)getpid(), (long)getppid());
	
	// Repeat for i from 1 to 3
	for(int i=0; i<3; i++){
		printf("[new child] %d\n", i+1);
		sleep(1);
	}
	return 0;
}
