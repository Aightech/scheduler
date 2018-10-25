/*
 * Creation date: thur, 25 oct 2018
 * Licence: GPL
 * Main authors:
 *   - Alexis Devillard <github.com/Aightech>
 *
 * Scheduler simulator.
 * 
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "task.h"

#define PREEMPT
#define DEFFERABLE_SERVER_ELECT 0
#define EDF_ELECT 1
#define STATIC_PRIORITY_ELECT 2
#define ROUND_ROBIN_ELECT 3


// Choose best task to wake up
//Round robin ord
task_t *elect();

void next_clock();

// Schedule your system
// Place this in clock interrupt if you want a preemptive schedule.
// To avoid complexity, we don't use clock interrupt, but explicitly call it from tasks
void sched();

#endif

