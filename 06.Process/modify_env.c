/* modify_env.c

   Demonstrate modification of the process environment list.

   Usage: modify_env name=value...

   Note: some UNIX implementations do not provide clearenv(), setenv(),
   and unsetenv().



   $ ./modify_env "GREET=Guten Tag" SHELL=/bin/bash BYE=Ciao
   GREET=Guten Tag
   SHELL=/bin/bash
   $ ./modify_env SHELL=/bin/sh BYE=byebye
   SHELL=/bin/shGREET=Hello world”

*/
#define _GNU_SOURCE     /* Get various declarations from <stdlib.h> */
#include <stdlib.h>
#include "tlpi_hdr.h"

#define NO_OVERRIDE 0
#define DO_OVERRIDE 1

extern char **environ;
int
main(int argc, char *argv[])
{
    int j;
    char **ep;

#ifndef __APPLE__
    clearenv();         /* Erase entire environment */
#endif

    /* Add any definitions specified on command line to environment */

    for (j = 1; j < argc; j++)
        if (putenv(argv[j]) != 0)
            errExit("putenv: %s", argv[j]);

    /* Add a definition for GREET if one does not already exist */

    if (setenv("GREET", "Hello world", NO_OVERRIDE) == -1)
        errExit("setenv");

    /* Remove any existing definition of BYE */

    unsetenv("BYE");

    /* Display current environment */

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    exit(EXIT_SUCCESS);
}
