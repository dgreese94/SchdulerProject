/*
 * schedulerlog.h
 *
 *  Created on: Sep 23, 2016
 *      Author: dgr3547
 */

#ifndef SCHEDULERLOG_H_
#define SCHEDULERLOG_H_

#define MAX_LOGENTRIES (200000)

typedef struct log_entry {
	unsigned long simulation_time;
	unsigned int priority;
	unsigned int process_id;
	char * description;
}log_entry;

void WriteLog();
void LogEntry(unsigned long simulation_time,
		      unsigned int priority,
			  unsigned int process_id,
			  char * description);

#endif /* SCHEDULERLOG_H_ */
