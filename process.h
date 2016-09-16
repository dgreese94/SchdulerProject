/*
 * process.h
 *
 *  Created on: Sep 15, 2016
 *      Author: dgr3547
 */

#include <pthread.h>

#ifndef PROCESS_H_
#define PROCESS_H_

#define MAX_THREADS 8
#define TIMESPEC_NS 1000000
#define TIMESPEC_SEC 0
#define HIGHEST_PRIORITY 10
#define IDLE_PRIORITY 2

typedef struct process_args {
	unsigned int exec_time;
	unsigned int period_time;
	unsigned int deadline;
}process;

pthread_t CreateProcess( process * args );
pthread_t CreateIdleThread();


#endif /* PROCESS_H_ */
