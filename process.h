/*
 * process.h
 *
 *  Created on: Sep 15, 2016
 *      Author: dgr3547
 */

#include <pthread.h>
#include <stdio.h>

#ifndef PROCESS_H_
#define PROCESS_H_

#define MAX_THREADS 8
#define TIMESPEC_NS 1000000
#define TIMESPEC_SEC 0
#define SCHEDULER_PRIORITY 10
#define HIGHEST_PRIORITY 9
#define READY_PRIORITY (HIGHEST_PRIORITY - 1)
#define LOWEST_PRIORITY (3)
#define IDLE_PRIORITY 2
#define DONE_PRIORITY (1)

//pthread_mutex_t start_lock;

typedef struct process_args {
	unsigned int exec_time;
	unsigned int period_time;
	unsigned int deadline;
	unsigned long next_period;
	unsigned int time_to_completion;
	unsigned long next_deadline;
	unsigned int process_id;
	pthread_t thread;
}process;

int terminated;

void StartingThreads();
void TerminateThreads();
pthread_t CreateProcess( process * args );
pthread_t CreateIdleThread();


#endif /* PROCESS_H_ */
