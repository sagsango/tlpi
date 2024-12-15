/* mq_notify_thread.c

   Demonstrate message notification via threads on a POSIX message queue.
*/
#include <pthread.h>
#include <mqueue.h>
#include <signal.h>
#include <fcntl.h>              /* For definition of O_NONBLOCK */
#include "tlpi_hdr.h"

/* This program does not handle the case where a message already exists on
   the queue by the time the first attempt is made to register for message
   notification. In that case, the program would never receive a notification.
   See mq_notify_via_thread.c for an example of how to deal with that case. */

static void notifySetup(mqd_t *mqdp);
static void                     /* Thread notification function */
threadFunc(union sigval sv)
{
	printf("mq_notify_thread.c::threadFunc()::pid = %ld\n", (long) getpid() );
    ssize_t numRead;
    mqd_t *mqdp;
    void *buffer;
    struct mq_attr attr;

    mqdp = sv.sival_ptr;

    /* Determine mq_msgsize for message queue, and allocate an input buffer
       of that size */

    if (mq_getattr(*mqdp, &attr) == -1)
        errExit("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        errExit("malloc");

    /* Reregister for message notification */

    notifySetup(mqdp);

    while ((numRead = mq_receive(*mqdp, buffer, attr.mq_msgsize, NULL)) >= 0)
        printf("Read %ld bytes\n", (long) numRead);
        /*FIXME: above: should use %zd here, and remove (long) cast */

    if (errno != EAGAIN)                        /* Unexpected error */
        errExit("mq_receive");

    free(buffer);
}
static void
notifySetup(mqd_t *mqdp)
{
	printf("mq_notify_thread.c::notifySetup()::pid = %ld\n", (long) getpid() );

    struct sigevent sev;

    sev.sigev_notify = SIGEV_THREAD;            /* Notify via thread */
    sev.sigev_notify_function = threadFunc;
    sev.sigev_notify_attributes = NULL;
            /* Could be pointer to pthread_attr_t structure */
    sev.sigev_value.sival_ptr = mqdp;           /* Argument to threadFunc() */

    if (mq_notify(*mqdp, &sev) == -1)
        errExit("mq_notify");
}
int
main(int argc, char *argv[])
{
	printf("mq_notify_thread.c::main()::pid = %ld\n", (long) getpid() );
    mqd_t mqd;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s mq-name\n", argv[0]);

    mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
    if (mqd == (mqd_t) -1)
        errExit("mq_open");

    notifySetup(&mqd);
    pause();                    /* Wait for notifications via thread function */
}
