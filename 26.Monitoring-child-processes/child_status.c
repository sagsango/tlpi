/* child_status.c

   Demonstrate the use of wait() and the W* macros for analyzing the child
   status returned by wait()

   Usage: child_status [exit-status]

   If "exit-status" is supplied, then the child immediately exits with this
   status. If no command-line argument is supplied then the child loops waiting
   for signals that either cause it to stop or to terminate - both conditions
   can be detected and differentiated by the parent. The parent process
   repeatedly waits on the child until it detects that the child either exited
   normally or was killed by a signal.


   $ ./child_status 23
   Child started with PID = 15807
   waitpid() returned: PID=15807; status=0x1700 (23,0)
   child exited, status=23


   $ ./child_status &
   [1] 15870
   $ Child started with PID = 15871
   kill -STOP 15871
   $ waitpid() returned: PID=15871; status=0x137f (19,127)
   child stopped by signal 19 (Stopped (signal))
   kill -CONT 15871
   $ waitpid() returned: PID=15871; status=0xffff (255,255)
   child continued


   kill -ABRT 15871
   $ waitpid() returned: PID=15871; status=0x0006 (0,6)
   child killed by signal 6 (Aborted)
   Press Enter, in order to see shell notification that background job has terminated
   [1]+  Done              ./child_status
   $ ls -l core
   ls: core: No such file or directory
   $ ulimit -c                                   Display RLIMIT_CORE limit
   0

   $ ulimit -c unlimited                           Allow core dumps
   $ ./child_status &
   [1] 15902
   $ Child started with PID = 15903
   kill -ABRT 15903                                Send SIGABRT to child
   $ waitpid() returned: PID=15903; status=0x0086 (0,134)
   child killed by signal 6 (Aborted) (core dumped)
   Press Enter, in order to see shell notification that background job has terminated
   [1]+  Done              ./child_status
   $ ls -l core                                    This time we get a core dump
   -rw-------   1 mtk      users       65536 May  6 21:01 core
*/


#include <sys/wait.h>
#include "print_wait_status.h"          /* Declares printWaitStatus() */
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    int status;
    pid_t childPid;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [exit-status]\n", argv[0]);

    switch (fork()) {
    case -1: errExit("fork");

    case 0:             /* Child: either exits immediately with given
                           status or loops waiting for signals */
        printf("Child started with PID = %ld\n", (long) getpid());
        if (argc > 1)                   /* Status supplied on command line? */
            exit(getInt(argv[1], 0, "exit-status"));
        else                            /* Otherwise, wait for signals */
            for (;;)
                pause();
        exit(EXIT_FAILURE);             /* Not reached, but good practice */

    default:            /* Parent: repeatedly wait on child until it
                           either exits or is terminated by a signal */
        for (;;) {
            childPid = waitpid(-1, &status, WUNTRACED
#ifdef WCONTINUED       /* Not present on older versions of Linux */
                                                | WCONTINUED
#endif
                    );
            if (childPid == -1)
                errExit("waitpid");

            /* Print status in hex, and as separate decimal bytes */

            printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n",
                    (long) childPid,
                    (unsigned int) status, status >> 8, status & 0xff);
            printWaitStatus(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
                exit(EXIT_SUCCESS);
        }
    }
}
