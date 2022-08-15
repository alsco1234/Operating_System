#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <pthread.h>
// TO DO: if necessary, include more header files

#include "Console.h"

#define TRUE 1
#define FALSE 0

#define MAX_THREAD 30

typedef struct {
	int thread_idx;
	int no_thread;
	int screen_height;
	int x;
	int delay;

	// TO DO: add fields to pass the vertical coordinates of the critical region
	int critical[2];

} ThreadParam;

int thread_cont = TRUE;			// global variable to control the loop in the thread function

void* ThreadFn(void *vParam);

// TO DO: declare a mutex or semaphore as a global variable
pthread_mutex_t mutex0;
pthread_mutex_t mutex1;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	clrscr();

	int no_thread = 0;
	if(argc > 1)
		no_thread = atoi(argv[1]);

	if(no_thread > MAX_THREAD)
		no_thread = MAX_THREAD;
	else if(no_thread == 0)
		no_thread = 5;

	// TO DO: initialize the mutex or semaphore.
	//		on failure, display an error message and quit
	if( pthread_mutex_init(&mutex0, NULL) ||
			pthread_mutex_init(&mutex1, NULL)) {
		printf("mutex initialize error.\n");
		exit(1);
	}

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	// the top and bottom coordinates of the critical region
	int critical_top = screen_height / 3;
	int critical_bottom = screen_height * 2 / 3;

	// mark the critical region by horizontal lines
	DrawLine(1, critical_top, screen_width, critical_top, '-');
	DrawLine(1, critical_bottom, screen_width, critical_bottom, '-');
	
	pthread_t tid[MAX_THREAD];
	ThreadParam param[MAX_THREAD];

	for(int i = 0; i < no_thread; i++){
		param[i].thread_idx = i;
		param[i].no_thread = no_thread;
		param[i].screen_height = screen_height;
		param[i].x = screen_width * (i + 1) / (no_thread + 1);
		param[i].delay = rand() % 300;

		// TO DO: add code to store the vertical coordinates of the critical region in param[i]
		param[i].critical[0] = critical_top;
		param[i].critical[1] = critical_bottom;

		pthread_create(&tid[i], NULL, ThreadFn, &param[i]);
	}

	PrintXY(1, screen_height + 1, "Press Enter to quit!\n");
	getchar();

	// termainte the threads
	thread_cont = FALSE;

	for(int i = 0; i < no_thread; i++)
		pthread_join(tid[i], NULL);

	// TO DO: destroy mutex or semaphore
	pthread_mutex_destroy(&mutex0);
	pthread_mutex_destroy(&mutex1);

	clrscr();
	gotoxy(1, 1);
	printf("Bye!\n");

	return 0;
}

void* ThreadFn(void *vParam)
{
	ThreadParam *param = (ThreadParam*)vParam;

	int y = 1;
	int oldy = 1;

	while(thread_cont){
		// TO DO: implement entry section here 
		if(y == param->critical[0]){
			pthread_mutex_lock(&mutex0);
			sleep(1);
		}
		else if(y == param->critical[1]){
			pthread_mutex_lock(&mutex1);
			sleep(1);
		}

		gotoxy(param->x, oldy);
		putchar(' ');

		gotoxy(param->x, y);
		putchar('*');

		fflush(stdout);

		// TO DO: implement exit section here
		if(y == param->critical[0])
			pthread_mutex_unlock(&mutex0);
		else if(y == param->critical[1])
			pthread_mutex_unlock(&mutex1);

		oldy = y;
		y++;
		if(y > param->screen_height)
			y = 1;

		gotoxy(1, 1);
		fflush(stdout);
		usleep(param->delay * 1000);
	}

	// TO DO: if current broke loop in the critical region, unlock mutex
	if(y==param->critical[0])
		pthread_mutex_unlock(&mutex0);
	else if(y==param->critical[1])
		pthread_mutex_unlock(&mutex1);

	return NULL;
}

