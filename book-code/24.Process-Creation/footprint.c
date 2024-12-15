/* footprint.c

   Using fork() + wait() to control the memory footprint of an application.

   This program contains a function that (artificially) consumes a large
   amount of memory. To avoid changing the process's memory footprint, the
   program creates a child process that calls the function. When the child
   terminates, all of its memory is freed, and the memory consumption of
   the parent is left unaffected.
*/


/* TODO:
   sbrk(int incrBytes): increment the heap size by given no of bytes.
                        returns the new break point(where heap ends).

                        so sbrk(0), will give break pioint.
                        break point = top of the heap.
                        
                        see man pages for more details.
*/


#ifdef __linux__
#define _DEFAULT_SOURCE
#else
#define _BSD_SOURCE     /* To get sbrk() declaration from <unistd.h> in case
                           _XOPEN_SOURCE >= 600; defining _SVID_SOURCE or
                           _GNU_SOURCE also suffices */
#endif

#include <sys/wait.h>
#include "tlpi_hdr.h"
static int
func(int arg)
{
    int j;

    for (j = 0; j < 0x100; j++)
        if (malloc(0x8000) == NULL)
            errExit("malloc");
    printf("Program break in child:  %10p\n", sbrk(0));

    return arg;
}
int
main(int argc, char *argv[])
{
    int arg = (argc > 1) ? getInt(argv[1], 0, "arg") : 0;
    pid_t childPid;
    int status;

    setbuf(stdout, NULL);           /* Disable buffering of stdout */

    printf("Program break in parent: %10p\n", sbrk(0));

    childPid = fork();
    if (childPid == -1)
        errExit("fork");

    if (childPid == 0)              /* Child calls func() and */
        exit(func(arg));            /* uses return value as exit status */

    /* Parent waits for child to terminate. It can determine the
       result of func() by inspecting 'status' */

    if (wait(&status) == -1)
        errExit("wait");

    printf("Program break in parent: %10p\n", sbrk(0));

    printf("Status = %d %d\n", status, WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
}
