/*
 * schedulerlog.c
 *
 *  Created on: Sep 30, 2016
 *      Author: dgr3547
 */

#include "schedulerlog.h"
#include <stdio.h>

log_entry schedule_log[MAX_LOGENTRIES];
long last_index = -1;

FILE * logfile;


void WriteLog(){
	logfile = fopen("scheduler.log", "w");
	long i;
	for (i = 0; i<=last_index;i++){
		fprintf(logfile, "%lu;%u;%u;%s;\n",
					schedule_log[i].simulation_time,
					schedule_log[i].process_id,
					schedule_log[i].priority,
					schedule_log[i].description);
		fflush(logfile);
	}

	fclose(logfile);
	return;
}

void LogEntry(unsigned long simulation_time, unsigned int priority,
			  unsigned int process_id, char * description)
{
	if(last_index+1<MAX_LOGENTRIES){
		schedule_log[last_index+1].simulation_time = simulation_time;
		schedule_log[last_index+1].process_id = process_id;
		schedule_log[last_index+1].priority = priority;
		schedule_log[last_index+1].description = description;

		last_index++;
	}

	return;
}
