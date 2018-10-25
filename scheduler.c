/*
 * Creation date: thur, 25 oct 2018
 * Licence: GPL
 * Main authors:
 *   - Alexis Devillard <github.com/Aightech>
 *
 * From the work of
 *   - Jérôme Pouiller <jerome@sysmic.org>
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




extern task_t* current_cntxt;
extern task_t main_cntxt;
/// List of all our tasks to be scheduled
extern task_t tasks[MAX_TASKS];

/// Clock counter
extern int clock;

/// How much clock count should we simulate before to exit?
extern int simul_length;

extern int round_robin;

extern int elect_mode;

task_t* defferable_server_elect()
{
	int i;
	int minP = 1000, minBis = 1000;
	task_t *electTask=NULL, *electTaskBis=NULL;
	task_t *bgTask=NULL;
	for (i = 0; i < MAX_TASKS; i++)
		if (tasks[i].state == running)//if the task is active
		{
			if(tasks[i].period == APERIODIC)//if the task is aperiodic it is put in the background list
				bgTask = &tasks[i];
			else if(tasks[i].period < minP)//if the task is periodic and have a minimal period
			{
				electTask = &tasks[i];
				minP = tasks[i].period;
			}
			
			//finally we seach for the minimal peridod among the task (without the server)
			if(i!=SERVER_TASK && tasks[i].period != APERIODIC && tasks[i].period < minBis)
			{
				electTaskBis= &tasks[i];
				minBis=tasks[i].period;
			}
		}

	if(electTask)//if a running task was found
	{
		if(electTask != tasks)//if the chosen task is not the server
			return electTask;//return the task
		else//if the server is the chosen task
		{
			if(bgTask)//if there are aperiodic task to execute
			{
				tasks[SERVER_TASK].capacity--;//discrease the capacity of the server
				if(!tasks[SERVER_TASK].capacity)//if the server capacity has been reach we stopped terminate the server
				{
					electTask->state = stopped;
					tasks[SERVER_TASK].capacity=SERVER_CAPACITY;
				}
			}
			else
				return electTaskBis;//if the server has no task to run, try to launch the other min period task
		}
	}
	if(bgTask)//if no periodic task were found
	return bgTask;
	return NULL;
}

task_t* EDF_elect()
{
	int i;
	int minD = 1000;//the minimal deadline found in the running tasks
	task_t* electTask=NULL;
	task_t *bgTask=NULL;//one of the background task
	for (i = 0; i < MAX_TASKS; i++)
		if (tasks[i].state == running)
		{
			if(tasks[i].deadline < minD)//test the min deadline
			{
				electTask = &tasks[i];
				minD = tasks[i].deadline;
			}
		}
	if(electTask)
		return electTask;
	return NULL;
}

task_t* static_priority_elect()
{
	int i, minP = 21;//the minimal deadline found in the running tasks
	task_t *electTask=NULL;//the elected task
	for (i = 0; i < MAX_TASKS; i++)
		if (tasks[i].state == running)
		{
			if(tasks[i].priority < minP)//test the min deadline
			{
				electTask = &tasks[i];
				minP = tasks[i].priority;
			}
		}
	if(electTask)
		return electTask;
	return  NULL;
}

task_t* round_robin_elect()
{
	int curRR = round_robin;//save the curent round robin
	for (;round_robin  < MAX_TASKS; round_robin++)//increase the round robin until a running task is find 
	if (tasks[round_robin].state == running)
		return &tasks[round_robin++];//run the task and increase the round robin

	//if no task has been found at the end of the array, look into the begining, start at 1 to avoid server task
	for (round_robin = 1; round_robin < curRR; round_robin++)
		if (tasks[round_robin].state == running)
			return &tasks[round_robin++];
	return NULL;
}

// Choose best task to wake up
//Round robin ord
task_t *elect()
{
	switch(elect_mode)
	{
		case DEFFERABLE_SERVER_ELECT: return defferable_server_elect();
		case EDF_ELECT: return EDF_elect();
		case STATIC_PRIORITY_ELECT: return static_priority_elect();
		case ROUND_ROBIN_ELECT: return round_robin_elect();
	}
}

void next_clock()
{
	int i;
	int shift;

	clock++;
	for (i = 0; i < MAX_TASKS; i++) 
	{
		if (tasks[i].capacity != EMPTY_SLOT)
		{
			shift = clock - tasks[i].begin;
			if(tasks[i].deadline < clock && tasks[i].state == running)
				printf("  Task %d did not respect deadline ( important failure)", i);
			if (!shift || (tasks[i].period != APERIODIC && !(shift % tasks[i].period)))
			{
				if (tasks[i].state == running)
					if (i != SERVER_TASK)
						printf("  Task %d did not respect Periodic deadline (important failure)", i);
					else//reload the server
						tasks[SERVER_TASK].capacity=SERVER_CAPACITY;
				else
					printf("  Task %d wakup", i);
				tasks[i].state = running;
				tasks[i].deadline = clock +tasks[i].deadlineRelative;
			}
		}
	}
	printf("\n");
	if (clock == simul_length)
	exit(0);
	printf("%3d ", clock);
}

// Schedule your system
// Place this in clock interrupt if you want a preemptive schedule.
// To avoid complexity, we don't use clock interrupt, but explicitly call it from tasks
void sched() {
	task_t *swap = current_cntxt;

	while (! (current_cntxt = elect()))
	{
		printf("No task running      ");
		print_pad(3 * MAX_TASKS);
		next_clock();
	}
	swapcontext(&swap->cntxt, &current_cntxt->cntxt);
}



