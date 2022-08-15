#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MesgQueueParent2Child 1024	// replace this number with an arbitrary number
#define MesgQueueChild2Parent 1025	// replace this number with an arbitrary number


// TO DO: declare a structure for messages
struct  {
	long type;
	int sum;
	int num;
} msg_data;

void Parent(int msgq_snd, int msgq_rcv);
int Child(int msgq_rcv, int msgq_snd);

int main()
{	
	// TO DO: Create two messsage queues using the keys MesgQueueParent2Child and MesgQueueChild2Parent
	// 		  On failure, display an error message
	int msgq_p2c;
	if((msgq_p2c = msgget(MesgQueueParent2Child, IPC_CREAT|0660)) == -1){
		printf("Failed to create queue.\n");
		return -1;
	}

	int msgq_c2p;
	if((msgq_c2p = msgget(MesgQueueChild2Parent, IPC_CREAT|0660)) == -1){
		printf("Faild to create queue.\n");
		return -1;
	}

	pid_t child_pid = fork();
	if(child_pid < 0){
		printf("Failed to create process.\n");
		return -1;
	} else if(child_pid > 0){
		Parent(msgq_p2c, msgq_c2p);						// DO NOT modify this line

		// TO DO: Destroy the two message queues.
		msgctl(msgq_p2c, IPC_RMID, 0);
		msgctl(msgq_c2p, IPC_RMID, 0);

		printf("Parent terminating.\n");				// This smessage should be displayed.

	} else {
		int sum = Child(msgq_p2c, msgq_c2p);			// DO NOT modify this line
		printf("Child terminating. sum = %d\n", sum);	// This smessage should be displayed.
	}

	return 0;
}

void Parent(int msgq_snd, int msgq_rcv)
// TO DO: implement this function
{
	/* TO DO:
        - Repeat
            . Read an integer from the user.
            . Send the number to the child using msgsnd().
			  On failure, display an error message.
            . If the input number is negative, break the loop.
            . Receive the sum of the input numbers from the child using msgrcv().
			  On failure, display an error message.
            . Display the sum. Put a prefix "[parent]" to indicate it was printed by the parent.
	*/
	while(1){
		int num;
		printf("Input integer numbers and child will accumulate (-1 to finish).\n");
		scanf("%d", &msg_data.num);

		if( msgsnd(msgq_snd, &msg_data.num, sizeof(msg_data.num), 0) == -1){
			printf("send number error");
		}

		if(msg_data.num<0) break;

		if (msgrcv(msgq_rcv, &msg_data.sum, sizeof(msg_data.sum), 0, 0) == -1){
			printf("receive sum error.\n");
		}

		printf("[parent] sum = %d\n", msg_data.sum);
	}
}

int Child(int msgq_rcv, int msgq_snd)
// TO DO: implement this function
{
	/* TO DO:
		- Initialize sum by zero.
		- Repeat
			. Receive a number from the parent using msgrcv().
			  On failure, display an error message.
			. If the number is negative, break the loop.
			. Display the input number. Put a prefix "[child]" to indicate it was printed by the child.
			. Add the number to sum.
			. Send the sum to the parent using msgsnd(). 
			  On failure, display an error message.
		- Return sum.
	*/
	msg_data.sum = 0;

	while(1){
		if( msgrcv(msgq_rcv, &msg_data.num, sizeof(msg_data.num), 0, 0) == -1){
			printf("recieve error.\n");
		}

		if(msg_data.num < 0) break;

		printf("[child] input number = %d\n", msg_data.num);

		msg_data.sum += msg_data.num;

		if( msgsnd(msgq_snd, &msg_data.sum, sizeof(msg_data.sum), 0) == -1){
			printf("send error.\n");
		}
	}
	return msg_data.sum;
}

