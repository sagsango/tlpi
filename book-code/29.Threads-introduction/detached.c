/* detached_attrib.c

   An example of the use of POSIX thread attributes (pthread_attr_t):
   creating a detached thread.


NOTE:	
	detached all the threads can exicute independently, but as soon as
	main thread completes its exicution by (retun/exit), all resoucces are released.
  so use pthread_exit() instread.

TODO:
  ./detach 0 10 10 10 
  ./detach 10 0 10 10 

*/
#include <pthread.h>
#include "tlpi_hdr.h"
static void *
threadFunc(void *x)
{

  int sl_time = (int)x;
	printf("threadFucn says Hi!\n");
	sleep(sl_time);
  return x;
}
int
main(int argc, char *argv[])
{
    if( argc == 1 || strcmp(argv[1],"--help") == 0  ){
      printf("%s main_thread_sleep_time child_thread_[1]_sleep_time ... child_thread_[n]_sleep_time\n",argv[0]);
      exit(EXIT_SUCCESS);
    }

    int nCh = argc - 2;

    pthread_t thr[nCh];
    pthread_attr_t attr;
    int s;

    for(int i=0;i<nCh;++i){
    s = pthread_attr_init(&attr);       /* Assigns default values */
    if (s != 0)
        errExitEN(s, "pthread_attr_init");

    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (s != 0)
        errExitEN(s, "pthread_attr_setdetachstate");

    s = pthread_create(&thr[i], &attr, threadFunc, (void *) atol(argv[i+2]));
    if (s != 0)
        errExitEN(s, "pthread_create");	
    }

    // TODO: detach the main thread and see.
    pthread_detach(pthread_self());

		sleep(atol(argv[1])); 

    s = pthread_attr_destroy(&attr);    // No longer needed //

    // TODO: use pthread_exit instead exit.
#define exit pthread_exit
    exit(EXIT_SUCCESS);
}
