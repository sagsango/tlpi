/* multi_SIGCHLD.c

   Demonstrate the use of a handler for the SIGCHLD signal, and that multiple
   SIGCHLD signals are not queued while the signal is blocked during the
   execution of the handler.
*/
#include <signal.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "curr_time.h"
#include "tlpi_hdr.h"

/*
26.Monitoring-child-processes ❱❱❱ ./multi_SIGCHLD  1 1 1 1 1
00:03:43 Child 1 (PID=1049695) exiting
00:03:43 Child 2 (PID=1049696) exiting
00:03:43 Child 3 (PID=1049697) exiting
00:03:43 Child 4 (PID=1049698) exiting
00:03:43 handler: Caught SIGCHLD
00:03:43 handler: Reaped child 1049695 - child exited, status=0
00:03:43 Child 5 (PID=1049699) exiting
00:03:43 handler: Reaped child 1049696 - child exited, status=0
00:03:43 handler: Reaped child 1049697 - child exited, status=0
00:03:48 handler: returning
00:03:48 handler: Caught SIGCHLD
00:03:48 handler: Reaped child 1049698 - child exited, status=0
00:03:48 handler: Reaped child 1049699 - child exited, status=0
00:03:53 handler: returning
00:03:53 All 5 children have terminated; SIGCHLD was caught 2 times
*/

static volatile int numLiveChildren = 0;
                /* Number of children started but not yet waited on */
static void
sigchldHandler(int sig)
{
    int status, savedErrno;
    pid_t childPid;

    /* UNSAFE: This handler uses non-async-signal-safe functions
       (printf(), printWaitStatus(), currTime(); see Section 21.1.2) */

    savedErrno = errno;         /* In case we modify 'errno' */

    printf("%s handler: Caught SIGCHLD\n", currTime("%T"));

    /* Do nonblocking waits until no more dead children are found */

    while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) { /* XXX: WNOHANG */
        printf("%s handler: Reaped child %ld - ", currTime("%T"),
                (long) childPid);
        printWaitStatus(NULL, status);
        numLiveChildren--;
    }

    if (childPid == -1 && errno != ECHILD)
        errMsg("waitpid");

    sleep(5);           /* Artificially lengthen execution of handler */
    printf("%s handler: returning\n", currTime("%T"));

    errno = savedErrno;
}
int
main(int argc, char *argv[])
{
    int j, sigCnt;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s child-sleep-time...\n", argv[0]);

    setbuf(stdout, NULL);       /* Disable buffering of stdout */

    sigCnt = 0;
    numLiveChildren = argc - 1;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchldHandler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    /* Block SIGCHLD to prevent its delivery if a child terminates
       before the parent commences the sigsuspend() loop below */

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
        errExit("sigprocmask");

    /* Create one child process for each command-line argument */

    for (j = 1; j < argc; j++) {
        switch (fork()) {
        case -1:
            errExit("fork");

        case 0:         /* Child - sleeps and then exits */
            sleep(getInt(argv[j], GN_NONNEG, "child-sleep-time"));
            printf("%s Child %d (PID=%ld) exiting\n", currTime("%T"),
                    j, (long) getpid());
            _exit(EXIT_SUCCESS);

        default:        /* Parent - loops to create next child */
            break;
        }
    }

    /* Parent comes here: wait for SIGCHLD until all children are dead */

    sigemptyset(&emptyMask);
    while (numLiveChildren > 0) {
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
            errExit("sigsuspend");
        sigCnt++;
    }

    printf("%s All %d children have terminated; SIGCHLD was caught "
            "%d times\n", currTime("%T"), argc - 1, sigCnt);

    exit(EXIT_SUCCESS);
}
