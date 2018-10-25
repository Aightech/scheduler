/*
 * Creation date: thur, 25 oct 2018
 * Licence: GPL
 * Main authors:
 *   - Alexis Devillard <github.com/Aightech>
 *
 * Scheduler simulator.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "task.h"
#include "tool.h"

int pgcd(int a, int b)
{
  int r = 1;
  while(b!=0){
    r=a%b;
    a=b;
    b=r;
  }
  return a;
}

int ppcm(int a, int b)
{
  return abs(a*b)/pgcd(a,b);
}

float CPU_charge(task_t tasks[])
{
  float u=0;
  int i=0;
  for(i=0;i<MAX_TASKS;i++)
    {
      u+=((float)tasks[i].capacity)/tasks[i].period;
    }
  return u;
}

float ord_crit()
{
  return MAX_TASKS*(pow(2,1.0/MAX_TASKS)-1);
}


