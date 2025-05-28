/* fork_stdio_buf.c

XXX:
   Experiment with fork() and stdio buffering.

   Experiment these 2 cases:
   1> ./fork_stdio_buf 
   2> ./fork_stdio_buf > fork_stdio_buf.txt
*/

#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    printf("Hello world\n");
    write(STDOUT_FILENO, "Ciao\n", 5);

    if (fork() == -1)
        errExit("fork");

    /* Both child and parent continue execution here */

    exit(EXIT_SUCCESS);
}
