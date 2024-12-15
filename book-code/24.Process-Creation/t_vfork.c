/* t_vfork.c

   Demonstrate the use of vfork() to create a child process.

   Two features distinguish the vfork() system call from fork() and make it more efficient:
   • No duplication of virtual memory pages or page tables is done for the child process. 
     Instead, the child shares the parent’s memory until it either performs a successful exec()
     or calls _exit() to terminate.
   • Execution of the parent process is suspended until the child has performed an exec() or 
     _exit().
*/

#ifdef __linux__
#define _DEFAULT_SOURCE
#else
#define _BSD_SOURCE     /* To get vfork() declaration from <unistd.h>
                           in case _XOPEN_SOURCE >= 700 */
#endif

#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    int istack = 222;

    switch (vfork()) {
    case -1:
        errExit("vfork");

    case 0:             /* Child executes first, in parent's memory space */
        sleep(3);                   /* Even if we sleep for a while,
                                       parent still is not scheduled */
        write(STDOUT_FILENO, "Child executing\n", 16);
        istack *= 3;                /* This change will be seen by parent */
        _exit(EXIT_SUCCESS);

    default:            /* Parent is blocked until child exits */
        write(STDOUT_FILENO, "Parent executing\n", 17);
        printf("istack=%d\n", istack);
        exit(EXIT_SUCCESS);
    }
}
