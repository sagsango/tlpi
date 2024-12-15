/* svmsg_demo_server.c
   Demonstration System V message queue-based server.

TODO:
	* System V IPC objects have kernel persistence. 
	Once created, an object continues to exist until it 
	is explicitly deleted or the system is shut down. 
	This property of System V IPC objects can be advantageous.

		If program crashed and new instance is created then,
	old ipc will be giving unexpected result, so should
	use new one.

		Here we are first checking if ipc already exist, then
	reove that, now try to make new one untill make call
	is successfull.


	*	see API-summery.png
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

#define KEY_FILE "./some-path/some-file"
                                /* Should be an existing file or one
                                   that this program creates */
int
main(int argc, char *argv[])
{
    int msqid;
    key_t key;
    const int MQ_PERMS = S_IRUSR | S_IWUSR | S_IWGRP;   /* rw--w---- */

    /* Optional code here to check if another server process is
       already running */

    /* Generate the key for the message queue */

    key = ftok(KEY_FILE, 1);
    if (key == -1)
        errExit("ftok");

    /* While msgget() fails, try creating the queue exclusively */

    while ((msqid = msgget(key, IPC_CREAT | IPC_EXCL | MQ_PERMS)) == -1) {
        if (errno == EEXIST) {          /* MQ with the same key already
                                           exists - remove it and try again */
            msqid = msgget(key, 0);
            if (msqid == -1)
                errExit("msgget() failed to retrieve old queue ID");
            if (msgctl(msqid, IPC_RMID, NULL) == -1)
                errExit("msgget() failed to delete old queue");
            printf("Removed old message queue (id=%d)\n", msqid);

        } else {                        /* Some other error --> give up */
            errExit("msgget() failed");
        }
    }

    /* Upon loop exit, we've successfully created the message queue,
       and we can then carry on to do other work... */

    printf("Queue created with id %d\n", msqid);

    exit(EXIT_SUCCESS);
}
