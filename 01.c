#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0


// data members of monitor

enum State { THINKING = 0, HUNGRY = 1, EATING = 2 };

typedef struct {
	int no_phil;
	pthread_mutex_t mutex;
	int *state;				// to allocate array of State
	pthread_cond_t *self;		// to allocate array of pthread_cond_t
} DiningPhilosophers;

void Init(DiningPhilosophers *dp, int no_phil);
void Destroy(DiningPhilosophers *dp);
void PickUp(DiningPhilosophers *dp, int idx);
void PutDown(DiningPhilosophers *dp, int idx);
void Test(DiningPhilosophers *dp, int idx);


// structure for thread parameters
typedef struct {
	int idx;				// index of current philosopher
	DiningPhilosophers *dp;
} ThreadInfo;

void *Philosopher(void *vparam);		// thread function
int repeat = TRUE;


int main(int argc, char *argv[])
{
	if(argc < 3){
		printf("Usage: %s <no_phil> <duration>\n", argv[0]);
		return 0;
	}

	int no_phil = atoi(argv[1]);
	int duration = atoi(argv[2]);
	printf("no_phil = %d, duration = %d\n", no_phil, duration);

	srand(time(NULL));

	DiningPhilosophers dp;
	Init(&dp, no_phil);

	pthread_t *phil = (pthread_t *)malloc(no_phil * sizeof(pthread_t));
	ThreadInfo *info = (ThreadInfo *)malloc(no_phil * sizeof(ThreadInfo));
	if(phil == NULL || info == NULL){
		printf("Failed to allocate memory.\n");
		exit(-1);
	}

	// TO DO: create philosopher threads
	// 		  deliver the index of the each thread and the address of 'dp' through 'info'.
	for(int i=0; i<no_phil; i++){
		info[i].idx = i;
		Init(&dp, i);
		info[i].dp = &dp;

		pthread_create(&phil[i], NULL, *Philosopher, &info[i]);
	}

	sleep(duration);
	
	printf("Terminting threads...\n");
	// TO DO: terminate threads by setting 'repeat' to zero
	repeat = 0;


	// TO DO: wait until all philosopher threads terminate
	for(int i =0; i<no_phil; i++)
		pthread_join(phil[i], NULL);
	

	// TO DO: destroy all dynamically allocated memory blocks, including the ones in 'dp'
	for(int i=0; i<no_phil; i++)
		Destroy(info[i].dp);

	printf("Bye!\n");			// this message should be displayed.

	return 0;
}

void *Philosopher(void *vparam)
// the thread function for each philosopher
// DO NOT modify this function
{
	ThreadInfo *param = (ThreadInfo*)vparam;

	DiningPhilosophers *dp = param->dp;
	int idx = param->idx;
	int left = (idx + dp->no_phil - 1) % dp->no_phil;
	int right = (idx + 1) % dp->no_phil;

	while(repeat){
		// remainder section
		printf("Philosopher %d is thinking.\n", param->idx);
		fflush(stdout);
		sleep(rand() % 3 + 2);		// wait for 2 ~ 4 sec

		// entry section
		printf("Philosopher %d is Hungry.\n", param->idx);
		fflush(stdout);
		PickUp(param->dp, param->idx);

		// critical section

		// error check
		if(param->dp->state[left] == EATING || param->dp->state[right] == EATING){
			printf("ERROR! phil[%d] = %d, phil[%d] = %d, phil[%d] = %d\n",
				left, dp->state[left], param->idx, dp->state[param->idx], right, dp->state[right]);
			exit(-1);
		}

		printf("=== Philosopher %d is EATING.\n", param->idx);
		fflush(stdout);
		sleep(rand() % 3 + 1);		// wait for 1 ~ 3 sec

		printf("=== Philosopher %d FINISHED EATING.\n", param->idx);
		fflush(stdout);

		// exit section
		PutDown(param->dp, param->idx);
	}

	return NULL;
}

// monitor functions
void Init(DiningPhilosophers *dp, int no_phil)
{
	pthread_mutex_init(&dp->mutex, NULL);

	dp->state = (int*)malloc(no_phil * sizeof(int));
	dp->self = (pthread_cond_t*)malloc(no_phil * sizeof(pthread_cond_t));
	if(dp->state == NULL || dp->self == NULL){
		printf("Failed to allocate memory.\n");
		exit(-1);
	}

	for(int i = 0; i < no_phil; i++){
		dp->state[i] = THINKING;
		pthread_cond_init(&dp->self[i], NULL);
	}

	dp->no_phil = no_phil;
}

void Destroy(DiningPhilosophers *dp)
{
	pthread_mutex_destroy(&dp->mutex);

	free(dp->state);
	dp->state = NULL;

	for(int i = 0; i < dp->no_phil; i++)
		pthread_cond_destroy(&dp->self[i]);

	free(dp->self);
	dp->self = NULL;
}

void PickUp(DiningPhilosophers *dp, int idx)
{
	// TO DO: implement this function.
	//		if necessary, use pthread_mutex_t to ensure mutual exclusion
	//		use pthread_cond_t for the condition variables
	dp->state[idx] = HUNGRY;
	Test(dp, idx);
	if (dp->state[idx] != EATING)
		pthread_cond_wait(&dp->self[idx], &dp->mutex);
}

void PutDown(DiningPhilosophers *dp, int idx)
{
	// TO DO: implement this function.
	//		if necessary, use pthread_mutex_t to ensure mutual exclusion
	//		use pthread_cond_t for the condition variables
	dp->state[idx] = THINKING;
	Test(dp, (idx+ (dp->no_phil-1) % (dp->no_phil) ) );
	Test(dp, ( (idx+1) % (dp->no_phil) ) );
}

void Test(DiningPhilosophers *dp, int idx)
{
	// TO DO: implement this function.
	//		if necessary, use pthread_mutex_t to ensure mutual exclusion
	//		use pthread_cond_t for the condition variables
	if((dp->state[(idx + (dp->no_phil-1) ) % (dp->no_phil)] != EATING) &&
		(dp->state[idx] == HUNGRY) &&
		(dp->state[(idx + 1) % (dp->no_phil)] != EATING)) {
			dp->state[idx] = EATING;
			pthread_cond_signal(&dp->self[idx]);
		}
}
