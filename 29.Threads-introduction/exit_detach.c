/*
   Differance between exit and pthread_exit
   see exit.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void * 
pthread_start(void * arg){

  int sleep_time = (int)(arg);
 
  printf("Thread id [%ld], going to sleep for [%ld] sec.\n", pthread_self(), sleep_time);
  sleep(sleep_time);

  exit(EXIT_SUCCESS);

}


int main(int argc, char * argv[] ){

  if( argc == 1 || strcmp(argv[1],"--help") ==  0 ){
    printf("%s MainThreadSleepTime child[1]SleepTime .. child[n]SleepTime\n",argv[0]);
    return 0;
  }

  int nCh = argc - 2;
  pthread_t th[nCh];

  for(int i=0;i<nCh;++i){
    pthread_create(&th[i],NULL,pthread_start,(void*)atol(argv[i+2]));
  }

  for(int i=0;i<nCh;++i){
    pthread_detach(th[i]);
  }

  pthread_detach(pthread_self());

  int sleep_time = atol(argv[1]);
  printf("Process id [%ld], thread id [%ld], going to sleep for [%ld] sec.\n", getpid(), pthread_self(), sleep_time);

  sleep(sleep_time);

  exit(EXIT_SUCCESS);
}
