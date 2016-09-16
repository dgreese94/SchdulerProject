/*
 * scheduler.h
 *
 *  Created on: Sep 16, 2016
 *      Author: dgr3547
 */

#include "process.h"

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define TIMER_NS 1000000
#define TIMER_SEC 0

unsigned long sim_time;

void InitSimTimer();
int RateMonotonicScheduler(process * arr, ...);

#endif /* SCHEDULER_H_ */
