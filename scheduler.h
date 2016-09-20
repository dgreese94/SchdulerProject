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
#define RUN_TIME 30000

unsigned long sim_time;


int RateMonotonicScheduler(process * arr, int arr_len);

#endif /* SCHEDULER_H_ */
