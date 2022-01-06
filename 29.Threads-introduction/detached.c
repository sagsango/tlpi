/* detached_attrib.c

   An example of the use of POSIX thread attributes (pthread_attr_t):
   creating a detached thread.


NOTE:	
	detached all the threads can exicute independently, but as soon as
	first thread completes its exicution, all resoucces are released.
TODO: commentout sleep(30), see output.
*/
#include <pthread.h>
#include "tlpi_hdr.h"
static void *
threadFunc(void *x)
{

	sleep(5);
	printf("threadFucn says Hi!\n");
    return x;
}
int
main(int argc, char *argv[])
{
    pthread_t thr;
    pthread_attr_t attr;
    int s;

    s = pthread_attr_init(&attr);       /* Assigns default values */
    if (s != 0)
        errExitEN(s, "pthread_attr_init");

    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (s != 0)
        errExitEN(s, "pthread_attr_setdetachstate");

    s = pthread_create(&thr, &attr, threadFunc, (void *) 1);
    if (s != 0)
        errExitEN(s, "pthread_create");	
		sleep(30);
    s = pthread_attr_destroy(&attr);    // No longer needed //
		/*
    if (s != 0)
        errExitEN(s, "pthread_attr_destroy");

    s = pthread_join(thr, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join failed as expected");
		*/

    exit(EXIT_SUCCESS);
}
