#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define N_MAX 10000000
pthread_t th[N_MAX];
int status;

void *thread_routine( void * arg){


	return 0;
}

int main(){
	for(int i=0;i<N_MAX;++i){
		status = pthread_create(&th[i],NULL,&thread_routine,NULL);
		if( status != 0 ){
			perror("Thread creation");
			return 1;
		}
	}
	for(int i=0;i<N_MAX;++i){
		status = pthread_join(th[i],NULL);
		if( status != 0 ){
			perror("Thread join");
			return 2;
		}
	}

	return 0;
}
