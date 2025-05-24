/* closeonexec.c

   Demonstrate retrieving and updating of the file descriptor
   close-on-exec flag.
*/
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/*
27.Program-execution ❱❱❱ ./closeonexec 1
ls: write error: Bad file descriptor
27.Program-execution ❱❱❱ ./closeonexec
-rwxrwxr-x 1 ssing214 ssing214 22464 May 24 00:47 ./closeonexec
27.Program-execution ❱❱❱
*/
int main(int argc, char *argv[]) {
  int flags;

  if (argc > 1) {
    /*    if (fcntl(STDERR_FILENO, F_LOCK) < 0) {
          perror("Cant tke the file lock");
        }
        if (fcntl(STDERR_FILENO, F_ULOCK) < 0) {
          perror("Cant unlock the file");
        }
*/
    flags = fcntl(STDOUT_FILENO, F_GETFD); /* Fetch flags */
    if (flags == -1)
      errExit("fcntl - F_GETFD");

    flags |= FD_CLOEXEC; /* Turn on FD_CLOEXEC */

    if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1) /* Update flags */
      errExit("fcntl - F_SETFD");
  }

  execlp("ls", "ls", "-l", argv[0], (char *)NULL);
  errExit("execlp");
}
