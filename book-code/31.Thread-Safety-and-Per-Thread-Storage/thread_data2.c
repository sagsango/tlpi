
/*
   pthread_once() is called for every thread,
   only first time in exicution.

   that can be used for initialization and
   other purposes.

   for more detail, see man page & 13.2
   in The Linux Programming Interface.

TODO:
  author - sagsango
  please check if the program is having
  correct behavior, improve it.
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
  // this destructur will be called just after thed exit.
  printf("===>> destructor(): deallocating the thread Specific data for thread [%ld]\n", pthread_self());
  free(ptr);
}

static void thread_key_create(){
	pthread_t t_id = pthread_self();
	printf(" ======>>> thread_key_create(): t_id = %lu\n", (unsigned long) t_id);

  (void) pthread_key_create(&key, &destructor);
}

// Lets say 2 diff threas are doing some computations and result
// will be in the dynamically allocated buffer and buffer have
// to be return.

// I am thinking eveytime function can make a dynamic allocation of
// memory and will return that pointer, but in that case calling
// function have to free the memory.

// and also lets say thread with id tID calles this function 100
// times then 100 times, malloc have to be called.
// it seems beetter to allocate some space specific to thread.

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

    }else{
    printf("===>> Allocated space already. t_id = %lu\n", (unsigned long) t_id);
  }

  memset((char*)ptr,0,OBJECT_SIZE);
  sprintf(ptr,"%s%lld",(char*)args, (long long) pthread_self());


  sleep(5);
  printf("===>> Thread Specific data for thread [%ld] = [%s]\n", pthread_self(), (char*)(ptr) );

	return NULL;
}



static char *msg[] = {"First:", "Second:"};

static void *thread_main_routine(void *args){
  pthread_t t_id = pthread_self();
	printf("thread_main_routine(): t_id = %lu\n", (unsigned long) t_id);

  thread_routine(msg[0]);
  thread_routine(msg[1]);

  printf("Thread %lld is going to exit\n",(long long)t_id); 
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


