/*
 * scheduler.c
 *
 *  Created on: Sep 16, 2016
 *      Author: dgr3547
 */

#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include "scheduler.h"


void clockTickAction(){
	sim_time++;
}

void InitSimTimer(){

	sim_time = 0;

	struct sigevent         event;
	struct sigaction		action;
	struct itimerspec       itime;
	timer_t                 timer;

	action.sa_sigaction = &clockTickAction;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &action, NULL);
	SIGEV_SIGNAL_INIT(&event, SIGUSR1);

	itime.it_value.tv_sec = 0;
	itime.it_value.tv_nsec = TIMER_NS;
	itime.it_interval.tv_sec = 0;
	itime.it_interval.tv_nsec = TIMER_NS;

	timer_create(CLOCK_REALTIME, &event, &timer);
	timer_settime(timer, 0, &itime, NULL);

    return;
}

int RateMonotonicScheduler(process * arr, int arr_len){
	printf("Starting Rate Monotonic Scheduler with %d processes.\n", arr_len);
	InitSimTimer();
	int i;
	process temp;
	pthread_t threads[arr_len];
	int original_prio[arr_len];
	struct timespec when;
	when.tv_sec = TIMESPEC_SEC;
	when.tv_nsec= TIMESPEC_NS;
	for (i = 1; i < arr_len; i++){
		int j = i;
		while(j > 0 && arr[j-1].period_time > arr[j].period_time ){
			temp = arr[j];
			arr[j] = arr[j-1];
			arr[j-1] = temp;
			j--;
		}
	}
	StartingThreads();
	for(i = 0; i< arr_len; i++){
		threads[i] = CreateProcess(&(arr[i]));
		pthread_setschedprio(threads[i], HIGHEST_PRIORITY - i);
		original_prio[i] = HIGHEST_PRIORITY - i;
		arr[i].next_period = sim_time + arr[i].period_time;
	}

	while(sim_time < RUN_TIME){
		for(i = 0; i < arr_len; i++){
			if(sim_time>arr[i].next_period){
				pthread_setschedprio(threads[i], original_prio[i]);
				arr[i].next_period = sim_time + arr[i].period_time;
			}
		}
		nanosleep(&when, NULL);
	}

	TerminateThreads();

	return 0;
}
