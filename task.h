/*
 * Creation date: thur, 25 oct 2018
 * Licence: GPL
 * Main authors:
 *   - Alexis Devillard <github.com/Aightech>
 *
 * Scheduler simulator.
 * 
 */
#ifndef TASK_H
#define TASK_H
#include <ucontext.h>
typedef enum { stopped = 0, running} state_t;

/** structure prototype **/
typedef struct TASK_T task_t;
typedef struct MTX_T mtx_t;
struct TASK_T {
  ucontext_t cntxt;
  int begin;
  /// Set capacity to EMPTY_SLOT if tasks should be ignored
  int capacity;
  /// Set period to APERIODIC to create aperiodic tasks
  int period;
  int deadline;      
  int deadlineRelative;

  int ref_priority;
  int priority;

  mtx_t* mtx;
  state_t state;
};


#define MAX_TASKS 7

#define SERVER_CAPACITY 1
#define SERVER_TASK 0

/// Main struct for tasks
#define EMPTY_SLOT 0
#define APERIODIC 0



// Simulation task
// Artificialy increment clock to simulate elapsed time
void task(int num, int capacity);

void create_task(int num, int begin, int period, int capacity, int deadline, int priority, mtx_t* mtx);

#endif
