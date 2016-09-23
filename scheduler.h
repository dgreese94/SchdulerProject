/*
 * scheduler.h
 *
 *  Created on: Sep 16, 2016
 *      Author: dgr3547
 */

#include "process.h"


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define RUN_TIME 30000

int RateMonotonicScheduler(process * arr, int arr_len);
int EarliestDeadlineScheduler(process * arr, int arr_len);
int ShortestCompletionScheduler(process * arr, int arr_len);

#endif /* SCHEDULER_H_ */
