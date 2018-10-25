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
#include "display.h"


/// A small helper to print padding spaces in output messages
void print_pad(int sz) {
    int i;
    
    for (i = 0; i < sz; i++)
        printf(" ");
}



