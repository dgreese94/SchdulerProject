/*
 * process.c
 *
 *  Created on: Sep 15, 2016
 *      Author: dgr3547
 */

#include "process.h"
#include "stdio.h"
/**
 * Thread function to create a simulated process
 *
 * @param args {void *} process_args struct that contains the execution time.
 */
void * fake_process(void * args){

  	struct timespec when;
  	when.tv_sec = TIMESPEC_SEC;
  	when.tv_nsec= TIMESPEC_NS;
  	process * pargs = args;
	int i = 0;
	int exec_time = pargs->exec_time;

	while(i<exec_time){
		nanospin(&when);
		i++;
	}
	printf("Exiting thread.\n");
	return 0;
}

/**
 * Idle thread function
 *
 * Task to perform when no other threads need to run
 * @param args {void*} NULL
 */
void * idle_thread(void * args){
	for(;;){}
	return 0;
}

/**
 * Creates a simulation process
 *
 * @param int exec_time - the time the process takes to execute
 * @param int period - the period that the process has
 * @param int deadline - the time in which the exec_time must occur before
 *
 * @return pthread_t - the created thread.
 */
pthread_t CreateProcess(process * args){

	pthread_t temp;

	pthread_create( &temp, NULL, &fake_process, args );

	return temp;
}

/**
 * Creates the Idle process
 *
 * @return pthread_t - the created thread.
 */
pthread_t CreateIdleThread(){
	pthread_t idle;

	pthread_create( &idle, NULL, &idle_thread, NULL );
	pthread_setschedprio(idle, IDLE_PRIORITY);

	return idle;
}
