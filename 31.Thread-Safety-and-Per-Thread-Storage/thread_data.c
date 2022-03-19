
/*
   pthread_once() is called for every thread,
   only first time in exicution.

   that can be used for initialization and
   other purposes.

   for more detail, see man page & 13.2
   in The Linux Programming Interface.

TODO:
  valgrind gives memory leak, for all the
  memory allocated with malloc.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define THREAD_COUNT 2
#define OBJECT_SIZE 100


// pthread_once_t should be always static.
static pthread_once_t once_control = PTHREAD_ONCE_INIT;
static pthread_key_t key;


static void destructor(void *ptr){
  printf("===>> destructor(): deallocating the thread Specific data for thread [%ld]\n", pthread_self());
  free(ptr);
}

static void thread_key_create(){
	pthread_t t_id = pthread_self();
	printf(" ======>>> thread_key_create(): t_id = %lu\n", (unsigned long) t_id);

  (void) pthread_key_create(&key, &destructor);
}


static void *thread_routine(void *args){

	pthread_t t_id = pthread_self();
	printf("===>> thread_routine(): t_id = %lu\n", (unsigned long) t_id);

	pthread_once( &once_control, &thread_key_create);

  void * ptr;
  if ((ptr = pthread_getspecific(key)) == NULL) {
    printf("===>> Allocating space for the ptr. t_id = %lu\n", (unsigned long) t_id);
    //TODO: Not static, Not on Stack, On heap, Why?
    ptr = malloc(OBJECT_SIZE);
    (void) pthread_setspecific(key, ptr);

    memset((char*)ptr,0,OBJECT_SIZE);
    sprintf(ptr,"%lld",(long long) pthread_self());
  }else{
    printf("===>> Allocated space already. t_id = %lu\n", (unsigned long) t_id);
  }

  printf("===>> Thread Specific data for thread [%ld] = [%s]\n", pthread_self(), (char*)(ptr) );

	return NULL;
}



static void *thread_main_routine(void *args){
  pthread_t t_id = pthread_self();
	printf("thread_main_routine(): t_id = %lu\n", (unsigned long) t_id);

  thread_routine(args);
  thread_routine(args);

  pthread_exit(EXIT_SUCCESS);
}



int main(){
	pthread_t t[THREAD_COUNT];

	for(int i=0;i<THREAD_COUNT;++i){
		pthread_create(&t[i], NULL,  thread_main_routine, NULL);
	}

	sleep(2);

	for(int i=0;i<THREAD_COUNT;++i){
		pthread_join(t[i], NULL);
	}
  
	return 0;
}


