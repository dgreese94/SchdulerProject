/*
 * scheduler.c
 *
 *  Created on: Sep 16, 2016
 *      Author: dgr3547
 */
#include "scheduler.h"
#include "simtimer.h"
#include "schedulerlog.h"

int RateMonotonicScheduler(process * arr, int arr_len){
//	fprintf(logfile, "Starting Rate Monotonic Scheduler with %d processes.\n", arr_len);
	InitSimTimer();
	int i;
	process temp;
	pthread_t idle = CreateIdleThread();
	int original_prio[arr_len];
	struct timespec when;
	when.tv_sec = TIMESPEC_SEC;
	when.tv_nsec= TIMESPEC_NS;

	for (i = 0; i < arr_len; i++){
		arr[i].process_id = i;
	}
	// insertion sort
	for (i = 1; i < arr_len; i++){
		arr[i].process_id = i;
		int j = i;
		while(j > 0 && arr[j-1].period_time > arr[j].period_time ){
			temp = arr[j];
			arr[j] = arr[j-1];
			arr[j-1] = temp;
			j--;
		}
	}
	StartingThreads();
//	fprintf(logfile, "Simulated time;Process ID;Priority;Description\n");

	for(i = 0; i< arr_len; i++){
		arr[i].thread = CreateProcess(&(arr[i]));
		pthread_setschedprio(arr[i].thread, HIGHEST_PRIORITY - i);
		original_prio[i] = HIGHEST_PRIORITY - i;
		arr[i].next_period = sim_time + arr[i].period_time;
		LogEntry(sim_time, HIGHEST_PRIORITY-i, arr[i].process_id, "Initial Creation");
	}

	while(sim_time < RUN_TIME){
		for(i = 0; i < arr_len; i++){
			if(sim_time>arr[i].next_period){
				LogEntry(sim_time, original_prio[i], arr[i].process_id, "Rescheduled");
				pthread_setschedprio(arr[i].thread, original_prio[i]);
				arr[i].next_period = sim_time + arr[i].period_time;
			}
		}
		nanosleep(&when, NULL);
	}

	TerminateThreads();

	for(i = 0; i< arr_len; i++){
		pthread_join(arr[i].thread, 0);
	}
	pthread_join(idle, 0);
	return 0;
}

int EarliestDeadlineScheduler(process * arr, int arr_len){
	printf("Starting Earliest Deadline First Scheduler with %d processes.\n", arr_len);
	InitSimTimer();
	int i,j;
	process temp;
	pthread_t idle = CreateIdleThread();
	struct timespec when;
	when.tv_sec = TIMESPEC_SEC;
	when.tv_nsec= TIMESPEC_NS;

	for (i = 0; i < arr_len; i++){
		arr[i].process_id = i;
	}

	for (i = 1; i < arr_len; i++){
		int j = i;
		while(j > 0 && arr[j-1].deadline > arr[j].deadline ){
			temp = arr[j];
			arr[j] = arr[j-1];
			arr[j-1] = temp;
			j--;
		}
	}

	StartingThreads();
	for(i = 0; i< arr_len; i++){
		arr[i].thread = CreateProcess(&(arr[i]));
		pthread_setschedprio(arr[i].thread, HIGHEST_PRIORITY - i);
		arr[i].next_period = sim_time + arr[i].period_time;
		arr[i].next_deadline = sim_time + arr[i].deadline;
		LogEntry(sim_time, HIGHEST_PRIORITY-i, arr[i].process_id, "Initial Creation");
	}

	while(sim_time < RUN_TIME){
		nanosleep(&when, NULL);
		for(i = 0; i < arr_len; i++){
			if(sim_time>arr[i].next_period){
				arr[i].next_deadline = sim_time + arr[i].deadline;
				for (j = 1; j < arr_len; j++){
					int k = j;
					while(k > 0 && arr[k-1].next_deadline > arr[k].next_deadline ){
						temp = arr[k];
						arr[k] = arr[k-1];
						arr[k-1] = temp;
						k--;
					}
				}
				for(i = 0; i < arr_len; i++){
					pthread_setschedprio(arr[i].thread, HIGHEST_PRIORITY - 1 );
					LogEntry(sim_time, HIGHEST_PRIORITY - 1, arr[i].process_id, "Rescheduled");
				}
				arr[i].next_period = sim_time + arr[i].period_time;
			}
		}
	}

	TerminateThreads();

	for(i = 0; i< arr_len; i++){
		pthread_join(arr[i].thread, 0);
	}
	pthread_join(idle, 0);
	return 0;
}

int ShortestCompletionScheduler(process * arr, int arr_len){
	printf("Starting Shortest Completion Time Scheduler with %d processes.\n", arr_len);
	InitSimTimer();
	int i,j;
	process temp;
	pthread_t idle = CreateIdleThread();
	struct timespec when;
	when.tv_sec = TIMESPEC_SEC;
	when.tv_nsec= TIMESPEC_NS;

	for (i = 0; i < arr_len; i++){
		arr[i].process_id = i;
	}

	for (i = 1; i < arr_len; i++){
		int j = i;
		while(j > 0 && arr[j-1].exec_time > arr[j].exec_time ){
			temp = arr[j];
			arr[j] = arr[j-1];
			arr[j-1] = temp;
			j--;
		}
	}

	StartingThreads();

	for(i = 0; i < arr_len; i++){
		arr[i].thread = CreateProcess(&(arr[i]));
		pthread_setschedprio(arr[i].thread, HIGHEST_PRIORITY - 1 );
		arr[i].next_period = sim_time + arr[i].period_time;
		LogEntry(sim_time, HIGHEST_PRIORITY-i, arr[i].process_id, "Initial Creation");
	}

	while(sim_time < RUN_TIME){
		nanosleep(&when, NULL);
		for(i = 0; i < arr_len; i++){
			if(sim_time>arr[i].next_period){
				for (j = 1; j < arr_len; j++){
					int k = j;
					while(k > 0 && arr[k-1].time_to_completion > arr[k].time_to_completion ){
						temp = arr[k];
						arr[k] = arr[k-1];
						arr[k-1] = temp;
						k--;
					}
				}
				for(i = 0; i < arr_len; i++){
					pthread_setschedprio(arr[i].thread, HIGHEST_PRIORITY - 1 );
					LogEntry(sim_time, HIGHEST_PRIORITY - 1, arr[i].process_id, "Rescheduled");
				}
				arr[i].next_period = sim_time + arr[i].period_time;
			}
		}
	}

	TerminateThreads();

	for(i = 0; i< arr_len; i++){
		pthread_join(arr[i].thread, 0);
	}
	pthread_join(idle, 0);
	return 0;
}
