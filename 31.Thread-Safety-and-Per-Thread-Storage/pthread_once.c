/*
   pthread_once() is called for every thread,
   only first time in exicution.

   that can be used for initialization and
   other purposes.

   for more detail, see man page & 13.2
   in The Linux Programming Interface.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#define THREAD_COUNT 2

 
void pthread_once_routine(void){
	
	pthread_t t_id = pthread_self();
	printf(" ======>>> pthread_once_routine(): t_id = %lu\n", (unsigned long) t_id);

}

void static_once_routine(void){

	pthread_t t_id = pthread_self();
	printf(" ======>>> static_once_routine(): t_id = %lu\n", (unsigned long) t_id);

}

void *thread_routine(void *args){

	pthread_t t_id = pthread_self();
	printf("===>> thread_routine(): t_id = %lu\n", (unsigned long) t_id);

   // pthread_once_t should be always static.
	static pthread_once_t once_control = PTHREAD_ONCE_INIT;

	pthread_once( &once_control, &pthread_once_routine);

	static bool static_once_init = true;
	if( static_once_init ){
    //This block can be accessed by multiple thread.
    //Because static_once_init is critical section.
    //Its always better to use
    //pthread_once rather than 
    //static variable.
		static_once_routine();
		static_once_init = false;
	}

	return NULL;
}

void *thread_main_routine(void *args){
  pthread_t t_id = pthread_self();
	printf("thread_main_routine(): t_id = %lu\n", (unsigned long) t_id);

  thread_routine(args);
  thread_routine(args);

  pthread_exit(EXIT_SUCCESS);
}


int main(){

	pthread_t t[THREAD_COUNT];

	for(int i=0;i<THREAD_COUNT;++i){
		pthread_create(&t[i], NULL,  &thread_main_routine, NULL);
	}

	sleep(2);

	for(int i=0;i<THREAD_COUNT;++i){
		pthread_join(t[i], NULL);
	}
  
	return 0;
}


