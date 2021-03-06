#include <stdlib.h>
#include <stdio.h>
#include <sys/trace.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"
#include "schedulerlog.h"

#define CSV_LINE_MAX (64)
#define CSV_WORD_MAX (16)



//////


/* Converts an integer string, e.g. "3561" into the equivalent unsigned integer, e.g. 3561.  Does not convert floats or
   negative integers.  Does not check that the provided string contains only integer characters.  This must be checked
   before passing the string to strToUint.
*/
unsigned int strToUint(char *str) {
    unsigned int result = 0, place = 0, length = 0, cur = 0;

    // get string length
    while (str[length++] != '\0');
    length--; // remove the null character from the count

    while (cur < length) {
        char c = str[cur];
        place = c - 48; // convert ASCII decimal to equivalent int
        int i = 1;
        while (i++ < length-cur) {
            place = place * 10;
        }
        result += place;
        cur++;
    }

    return result;
}

int parseCSV(char *filePath, process * arr) {
    FILE *file = fopen(filePath, "r");
    int lineNo = -5;
    if (file != NULL) {
        // get a line
        char line[CSV_LINE_MAX];
        lineNo = 0;
        while (fgets(line, CSV_LINE_MAX, file) != NULL) {
            lineNo++;
            // create a new int array representing a process [execution time, period, deadline]
            process process_builder;

            // read each word
            char word[CSV_WORD_MAX];
            char c;
            int lineCur = 0, wordCur = 0, processCur = 0;
            while (line[lineCur] != '\0') {
                c = line[lineCur++];
                if (c == ',' || c == ';') {
                    word[wordCur] = '\0';
                    switch(processCur++){
						case 0:
							process_builder.exec_time = strToUint(word);
							break;
						case 1:
							process_builder.period_time = strToUint(word);
							break;
						case 2:
							process_builder.deadline = strToUint(word);
							break;
						default:
							break;
					}
                    wordCur = 0;
                }
                else if ((c < '0' || c > '9') && (c != '\n' && c != '\r')) {
                    // file content error
                    return -2;
                }
                else if (c != '\n' && c != '\r') {
                    if (processCur < 3) {
                        word[wordCur++] = c;
                    }
                    else {
                        // line format error
                        return -3;
                    }
                }
            }

            arr[lineNo - 1] = process_builder;
        }

        if (ferror(file)) {
            // line read error
            return -4;
        }
    }
    //else returns -5 file read error

    fclose(file);
    return lineNo;
}

int main(int argc, char *argv[]) {

	//readCSV
	if (argc < 2 || argc > 3) {
		// format error
		printf("Incorrect number of args. %d\n", argc);
		int i = 0;
		while (i++ < argc) {
			printf("arg %d: %s\n", i, argv[i-1]);
		}
		return 1;
	}
	else {
		process arr[MAX_THREADS];
		int parseCSVResult = parseCSV(argv[1], arr);
		if (parseCSVResult<0) {
			// conversion error
			printf("Failed to parse CSV\n");
			printf("%d\n", parseCSVResult);
			return parseCSVResult;
		}
		else {
			printf("CSV parsed successfully\n");

		}

		if(!strcmp(argv[2],"rm")){
			RateMonotonicScheduler(arr, parseCSVResult);
		}
		else if (!strcmp(argv[2], "edf")){
			EarliestDeadlineScheduler(arr, parseCSVResult);
		}
		else if (!strcmp(argv[2],"sct")){
			ShortestCompletionScheduler(arr, parseCSVResult);
		}
	}
	WriteLog();
	printf("Done writing Log. Exiting.\n");
	return EXIT_SUCCESS;
}
