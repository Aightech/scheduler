/*
 * Creation date: thur, 25 oct 2018
 * Licence: GPL
 * Main authors:
 *   - Alexis Devillard <github.com/Aightech>
 *
 * Scheduler simulator.
 * 
 */
#ifndef MUTEX_H
#define MUTEX_H

/** structure prototype **/
typedef struct TASK_T task_t;
typedef enum { unlocked = 0, locked} stateMtx_t;
typedef struct MTX_T mtx_t;
struct MTX_T{
  task_t* owning_task;
  stateMtx_t state;
};


#define EINVAL 1
#define EDEADLK 2
#define EPERM 2


//link the mutex with the current task
void m_create(mtx_t* mtx);

/**
returns:
- 0       | if the mutex got successfully locked
- EINVAL  | if the mutex hasn't been initialized
- EDEADLK | if the mutex is owned and locked by another tasked
 **/
int m_trylock(mtx_t* mtx);

void m_lock(mtx_t* mtx);


/**
returns:
- 0       | if the mutex got successfully unlocked
- EINVAL  | if the mutex hasn't been initialized
- EDEADLK | if the mutex is not owned by the task
 **/
int m_tryunlock(mtx_t* mtx);

void m_unlock(mtx_t* mtx);

#endif

