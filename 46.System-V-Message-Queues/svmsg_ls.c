/* svmsg_ls.c

   Display a list of all System V message queues on the system.

   This program is Linux-specific.

TODO:
    To list all message queues on the system, we can do the following:
    * Use a MSG_INFO operation to find out the maximum index (maxind) of the entries array 
    for message queues.
    * Perform a loop for all values from 0 up to and including maxind, employing a MSG_STAT
    operation for each value. During this loop, we ignore the errors that may occur if an 
    item of the entries array is empty (EINVAL) or if we don’t have permissions on the object
    to which it refers (EACCES).
*/

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/msg.h>
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    int maxind, ind, msqid;
    struct msqid_ds ds;
    struct msginfo msginfo;

    /* Obtain size of kernel 'entries' array */

    maxind = msgctl(0, MSG_INFO, (struct msqid_ds *) &msginfo);
    if (maxind == -1)
        errExit("msgctl-MSG_INFO");

    printf("maxind: %d\n\n", maxind);
    printf("index     id       key      messages\n");

    /* Retrieve and display information from each element of 'entries' array */

    for (ind = 0; ind <= maxind; ind++) {
        msqid = msgctl(ind, MSG_STAT, &ds);
        if (msqid == -1) {
            if (errno != EINVAL && errno != EACCES)
                errMsg("msgctl-MSG_STAT");              /* Unexpected error */
            continue;                                   /* Ignore this item */
        }

        printf("%4d %8d  0x%08lx %7ld\n", ind, msqid,
                (unsigned long) ds.msg_perm.__key, (long) ds.msg_qnum);
    }

    exit(EXIT_SUCCESS);
}
