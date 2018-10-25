/*
 * Creation date: thur, 25 oct 2018
 * Licence: GPL
 * Main authors:
 *   - Alexis Devillard <github.com/Aightech>
 *
 * Scheduler simulator.
 * 
 */
#ifndef TOOL_H
#define TOOL_H
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "task.h"

int pgcd(int a, int b);

int ppcm(int a, int b);

float CPU_charge(task_t tasks[]);

float ord_crit();
#endif
