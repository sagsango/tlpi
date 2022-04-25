/* is_echo_inetd_sv.c

   An inetd-invoked implementation of the TCP "echo" service.

   Compare this program with is_echo_sv.c.


TODO:
  Inetd perform all these steps, when service is required:
  1. Perform all socket-related initialization, calling socket(),
  bind(), and (for TCP servers) listen().
  2. For a TCP service, perform an accept() for the new connection.
  3. Create a new process to handle the incoming UDP datagram or TCP 
  connection. The process is automatically set up as a daemon. The 
  inetd program handles all details of process creation via fork() 
  and the reaping of dead children via a handler for SIGCHLD.
  4. Duplicate the file descriptor of the UDP socket or the connected
  TCP socket on file descriptors 0, 1, and 2, and close all other 
  file descriptors (since they are unused in the execed server).
  Exec the server program.
  5. Finally does exec(), and replace the code with bellow.

*/
#include <syslog.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 4096
int
main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, numRead) != numRead) {
            syslog(LOG_ERR, "write() failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        syslog(LOG_ERR, "Error from read(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
