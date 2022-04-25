/*  self_pipe.c

   Employ the self-pipe trick so that we can avoid race conditions while both
   selecting on a set of file descriptors and also waiting for a signal.

   Usage as shown in synopsis below; for example:

        self_pipe - 0
*/
#include <sys/time.h>
#if ! defined(__hpux)   /* HP-UX 11 doesn't have this header file */
#include <sys/select.h>
#endif
#include <fcntl.h>
#include <signal.h>
#include "tlpi_hdr.h"

static int pfd[2];                      /* File descriptors for pipe */
static void
handler(int sig)
{
    int savedErrno;                     /* In case we change 'errno' */

    savedErrno = errno;
    if (write(pfd[1], "x", 1) == -1 && errno != EAGAIN)
        errExit("write");
    errno = savedErrno;
}

