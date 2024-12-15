/* signal_functions.h

   Header file for signal_functions.c.
*/
#ifndef SIGNAL_FUNCTIONS_H
#define SIGNAL_FUNCTIONS_H

#include <signal.h>
#include "tlpi_hdr.h"

/* Print mask of blocked signals for this process */
int printSigMask(FILE *of, const char *msg);

/* Print signals currently pending for this process */
int printPendingSigs(FILE *of, const char *msg);

/* Print list of signals within a signal set */
void printSigset(FILE *of, const char *ldr, const sigset_t *mask);

#endif
