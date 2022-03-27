#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define N_MAX 1
#define FILE_NAME "./tmp.txt"
pthread_t th[N_MAX];
int status;

void *thread_routine( void * arg){

	int fd = open(FILE_NAME, O_CREAT | O_EXCL | O_RDWR );

	if( fd == -1 ){
		perror("open()");
		exit(EXIT_FAILURE);
	}
	if( unlink(FILE_NAME) != 0 ){
		perror("unlink(1)");
		exit(EXIT_FAILURE);
	}
  sleep(5);
	if( close(fd) != 0 ){
		perror("close()");
		exit(EXIT_FAILURE);
	}

	pthread_exit(EXIT_SUCCESS);
}

int main(){
	for(int i=0;i<N_MAX;++i){
		status = pthread_create(&th[i],NULL,&thread_routine,NULL);
		if( status != 0 ){
		perror("pthread_create()");
		exit(EXIT_FAILURE);
		}
	}
	for(int i=0;i<N_MAX;++i){
		status = pthread_join(th[i],NULL);
		if( status != 0 ){
			perror("pthread_join()");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}
