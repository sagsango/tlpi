/* multi_wait.c

   Demonstrate the use of wait(2): create multiple children and then wait
   for them all.

   Usage: multi_wait sleep-time...

   One child process is created for each command-line argument. Each child
   sleeps for the number of seconds specified in the corresponding command-line
   argument before exiting. After all children have been created, the parent
   loops, waiting for terminated children, and displaying their PIDs.
*/
#include "curr_time.h" /* Declaration of currTime() */
#include "tlpi_hdr.h"
#include <sys/wait.h>
#include <time.h>

/*
26.Monitoring-child-processes ❱❱❱ ./wating-for-childrens 1 1 1 1 1 1 1 1
[00:20:57] child 1 started with PID 1050259, sleeping 1 seconds
[00:20:57] child 2 started with PID 1050260, sleeping 1 seconds
[00:20:57] child 3 started with PID 1050261, sleeping 1 seconds
[00:20:57] child 4 started with PID 1050262, sleeping 1 seconds
[00:20:57] child 5 started with PID 1050263, sleeping 1 seconds
[00:20:57] child 7 started with PID 1050265, sleeping 1 seconds
[00:20:57] child 6 started with PID 1050264, sleeping 1 seconds
[00:20:57] child 8 started with PID 1050266, sleeping 1 seconds
[00:20:58] wait() returned child PID 1050259 (numDead=1)
[00:20:58] wait() returned child PID 1050260 (numDead=2)
[00:20:58] wait() returned child PID 1050261 (numDead=3)
[00:20:58] wait() returned child PID 1050262 (numDead=4)
[00:20:58] wait() returned child PID 1050263 (numDead=5)
[00:20:58] wait() returned child PID 1050265 (numDead=6)
[00:20:58] wait() returned child PID 1050264 (numDead=7)
[00:20:58] wait() returned child PID 1050266 (numDead=8)
No more children - bye!
*/

int main(int argc, char *argv[]) {
  int numDead;    /* Number of children so far waited for */
  pid_t childPid; /* PID of waited for child */
  int j;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s sleep-time...\n", argv[0]);

  setbuf(stdout, NULL); /* Disable buffering of stdout */

  for (j = 1; j < argc; j++) { /* Create one child for each argument */
    switch (fork()) {
    case -1:
      errExit("fork");

    case 0: /* Child sleeps for a while then exits */
      printf("[%s] child %d started with PID %ld, sleeping %s "
             "seconds\n",
             currTime("%T"), j, (long)getpid(), argv[j]);
      sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));
      _exit(EXIT_SUCCESS);

    default: /* Parent just continues around loop */
      break;
    }
  }

  numDead = 0;
  for (;;) { /* Parent waits for each child to exit */
    childPid = wait(NULL);
    if (childPid == -1) {
      if (errno == ECHILD) {
        printf("No more children - bye!\n");
        exit(EXIT_SUCCESS);
      } else { /* Some other (unexpected) error */
        errExit("wait");
      }
    }

    numDead++;
    printf("[%s] wait() returned child PID %ld (numDead=%d)\n", currTime("%T"),
           (long)childPid, numDead);
  }
}
