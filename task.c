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
#include "mutex.h"
#include "task.h"
#include "display.h"
#include "scheduler.h"

extern task_t* current_cntxt;
extern task_t main_cntxt;
/// List of all our tasks to be scheduled
extern task_t tasks[MAX_TASKS];



// Simulation task
// Artificialy increment clock to simulate elapsed time
void task(int num, int capacity) {
	int i;

	for (;;)
	{
		m_lock(current_cntxt->mtx);
		for (i = 0; i < capacity; i++)
		{
			
			printf("Task %2d (%2d  %2d  %2d), step %2d/%2d ", num,current_cntxt->deadline,current_cntxt->period, current_cntxt->priority, i + 1, capacity);
			print_pad(3 * num);
			printf("#%d ", i + 1);
			print_pad(3 * (MAX_TASKS - num - 1));
			if (i == capacity - 1)
				current_cntxt->state = stopped;
			next_clock();
			
			if (i == capacity - 1)
				m_unlock(current_cntxt->mtx);
			sched();
		}
	}
}

void create_task(int num, int begin, int period, int capacity, int deadline, int priority, mtx_t* mtx) {
  //alocate a context
  getcontext(&tasks[num].cntxt);
  tasks[num].cntxt.uc_stack.ss_sp = malloc(16 * 1024);
  tasks[num].cntxt.uc_stack.ss_size = 16 * 1024;
  tasks[num].cntxt.uc_link = &main_cntxt.cntxt;

  //set task's parameters
  tasks[num].capacity = capacity;
  tasks[num].period = period;
  tasks[num].state = stopped;
  tasks[num].begin = begin;

  //set deadlines
  tasks[num].deadlineRelative = begin+deadline;
  tasks[num].deadline = begin+deadline;

  //set priority
  tasks[num].priority = priority;
  tasks[num].ref_priority = priority;
  makecontext(&tasks[num].cntxt, (void (*)(void)) task, 2, num, capacity);

  //add the mutex or NULL to the task's mutex
  current_cntxt=&tasks[num];
  m_create(mtx);
  current_cntxt=NULL;
}


