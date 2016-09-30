/*
 * simtimer.c
 *
 *  Created on: Sep 23, 2016
 *      Author: dgr3547
 */

#include "simtimer.h"
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <time.h>

unsigned long sim_time;

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
