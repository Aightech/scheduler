/*
 * Creation date: Mon, 20 Dec 2010 07:57:13 +0100
 * Licence: GPL
 * Main authors:
 *   - Jérôme Pouiller <jerome@sysmic.org>
 *
 * Scheduler simulator.
 */
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mutex.h"
#include "task.h"
#include "scheduler.h"
#include "display.h"

extern task_t* current_cntxt;
extern task_t tasks[MAX_TASKS];



//link the mutex with the current task
void m_create(mtx_t* mtx)
{
  current_cntxt->mtx = mtx;
}


/**
returns:
- 0       | if the mutex got successfully locked
- EINVAL  | if the mutex hasn't been initialized
- EDEADLK | if the mutex is owned and locked by another tasked
 **/
int m_trylock(mtx_t* mtx)
{
	//if the mutex hasn't been initialized
	if(mtx==NULL)
		return EINVAL;

	//if the mutex is lock by another task
	if(mtx->state==locked && mtx->owning_task!=current_cntxt)
		return EDEADLK;

	//if evrything is fine, then get the mutex and lock it
	mtx->owning_task=current_cntxt;
	mtx->state=locked;
	return 0;
}

void m_lock(mtx_t* mtx)
{
	//if the mutex hasn't been initialized
	if(mtx==NULL)
		return (void) EINVAL;

	//while the mutex is lock by another task
	while(mtx->state==locked && mtx->owning_task!=current_cntxt)
	{
		//give its priority to the blocking task to avoid priority inversion
		mtx->owning_task->priority = current_cntxt->priority;
		printf("\t\t\t\t\t");
		print_pad(3 * MAX_TASKS);
		printf("Task %ld got task %ld's priority",mtx->owning_task-tasks,current_cntxt-tasks);
		int i;
		for(i=0;i<4 * MAX_TASKS+57;i++)
			printf("\b");
		
		sched();
	}

	//if evrything is fine, then get the mutex and lock it
	mtx->owning_task=current_cntxt;
	mtx->state=locked;
}


/**
returns:
- 0       | if the mutex got successfully unlocked
- EINVAL  | if the mutex hasn't been initialized
- EDEADLK | if the mutex is not owned by the task
 **/
int m_tryunlock(mtx_t* mtx)
{
	if(mtx==NULL)
		return EINVAL;
	if(mtx->owning_task!=current_cntxt)
		return EPERM;
	mtx->state=unlocked;
	mtx->owning_task=NULL;
	return 0;
}

void m_unlock(mtx_t* mtx)
{
	if(mtx==NULL)
		return (void)EINVAL;
	if(mtx->owning_task!=current_cntxt)
		return (void)EPERM;
	mtx->state=unlocked;
	mtx->owning_task=NULL;
}

