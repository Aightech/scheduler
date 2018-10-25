/*
 * Creation date: thur, 25 oct 2018
 * Licence: GPL
 * Main authors:
 *   - Alexis Devillard <github.com/Aightech>
 *
 * Scheduler simulator.
 * 
 */

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mutex.h"
#include "task.h"
#include "display.h"
#include "scheduler.h"
#include "tool.h"






/// List of all our tasks to be scheduled
task_t tasks[MAX_TASKS];

/// Currently running task
task_t *current_cntxt = NULL;
/// Clock counter
int clock = -1;

/// How much clock count should we simulate before to exit?
int simul_length = 30;

task_t main_cntxt;

int round_robin = 1;

int elect_mode=0;

void usage(char *ps)
{
	printf("./%s <mode of scheduler>\n",ps);
	printf("mode of scheduler:\n");
	printf("\t + %d = Defferable Server\n", DEFFERABLE_SERVER_ELECT);
	printf("\t + %d = EDF\n", EDF_ELECT);
	printf("\t + %d = Static Priority\n", STATIC_PRIORITY_ELECT);
	printf("\t + %d = Round Robin\n", ROUND_ROBIN_ELECT);
}

void set_elect_mode(int mode)
{
	elect_mode=mode;
}


int main(int argc, char *argv[]) {
	if(argc<2)
		usage(argv[0]);
	set_elect_mode(atoi(argv[1]));
    bzero(tasks, sizeof(tasks));
	mtx_t mtx1;
    //create a server so the selct method is called periodically every n turn (here 1) for a RR of n 
    create_task(SERVER_TASK, 0, 2, 0, 0, 0,NULL);
    //---------(ID,begin, period, capacity, deadline, priority, mutex)
    create_task(1, 0, 21, 6, 20, 20, &mtx1);
    create_task(2, 1, 21, 3, 20, 10, &mtx1);
    create_task(3, 0, 21, 4, 20, 20, NULL);
    printf("CLK|Task ID |(DL  P  NI)|  - STEPS -");
    print_pad(3 * MAX_TASKS);
    next_clock();
    // In case a task end, it will come back here
    getcontext(&main_cntxt.cntxt);
    current_cntxt = &main_cntxt;
    sched();
    // Never return but compiler make a warning
    return 0;
}

