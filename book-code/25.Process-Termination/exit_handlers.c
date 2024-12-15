/* exit_handlers.c

   Demonstrate the use of atexit(3) and on_exit(3), which can be used to
   register functions (commonly known as "exit handlers") to be called at
   normal process exit. (These functions are not called if the process does
   an _exit(2) or if it is terminated by a signal.)
*/

#ifdef __linux__
#define _DEFAULT_SOURCE
#else
#define _BSD_SOURCE     /* Get on_exit() declaration from <stdlib.h> */
#endif

#include <stdlib.h>
#include "tlpi_hdr.h"

#ifdef __linux__        /* Few UNIX implementations have on_exit() */
#define HAVE_ON_EXIT
#endif
static void
atexitFunc1(void)
{
    printf("atexit function 1 called\n");
}
static void
atexitFunc2(void)
{
    printf("atexit function 2 called\n");
}
#ifdef HAVE_ON_EXIT
static void
onexitFunc(int exitStatus, void *arg)
{
    printf("on_exit function called: status=%d, arg=%ld\n",
                exitStatus, (long) arg);
}
#endif
int
main(int argc, char *argv[])
{
#ifdef HAVE_ON_EXIT
    if (on_exit(onexitFunc, (void *) 10) != 0)
        fatal("on_exit 1");
#endif
    if (atexit(atexitFunc1) != 0)
        fatal("atexit 1");
    if (atexit(atexitFunc2) != 0)
        fatal("atexit 2");
#ifdef HAVE_ON_EXIT
    if (on_exit(onexitFunc, (void *) 20) != 0)
        fatal("on_exit 2");
#endif

    exit(2);
}
