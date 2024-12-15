/* pmsg_unlink.c

   Usage: pmsg_unlink mq-name

   Unlink a POSIX message queue.

   Linux supports POSIX message queues since kernel 2.6.6.
*/
#include <mqueue.h>
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s mq-name\n", argv[0]);

    if (mq_unlink(argv[1]) == -1)
        errExit("mq_unlink");
    exit(EXIT_SUCCESS);
}
