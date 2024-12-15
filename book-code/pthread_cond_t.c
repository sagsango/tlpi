#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#define nTHREAD 2
#define MAX 50
pthread_t t_producer[MAX], t_consumer;

pthread_mutex_t t_mutex;
pthread_cond_t t_cond;

static long long int shared_counter = 0;



void* fProducer(void* arg){
	pthread_mutex_lock(&t_mutex);

	int pIncr = atoi((const char*)arg);
	printf("fProducer Start: %lld for Incr:%d\n", shared_counter, pIncr);
	for(int i=0;i<pIncr;++i){
		shared_counter += 1;
	}
	printf("fProducer Ends: %lld\n", shared_counter);



	pthread_mutex_unlock(&t_mutex);
	pthread_cond_signal(&t_cond);
}

void* fConsumer(void* arg){

	pthread_mutex_lock(&t_mutex);
	while( shared_counter == 0 ){
		pthread_cond_wait(&t_cond,&t_mutex);
	}

	printf("fConsumer Start: %lld\n", shared_counter);
	while( shared_counter > 0 ){
		shared_counter -= 1;
	}
	printf("fConsumer Ends: %lld\n", shared_counter);



	pthread_mutex_unlock(&t_mutex);

}
int main(int argc, char * argv[]){
	pthread_mutex_init(&t_mutex,NULL);
	pthread_cond_init(&t_cond,NULL);

	if( argc == 1 ){
		printf("Error: Provide producer increment counts\n");
		return 0;
	}

	pthread_create(&t_consumer,NULL,&fConsumer,NULL);
	for(int i=1;i<argc;++i){
		pthread_create(&t_producer[i],NULL,&fProducer,(void*)argv[i]);
	}

	for(int i=1;i<=argc;++i){
		pthread_join(t_producer[i], NULL);
	}
	pthread_join(t_consumer, NULL);



}
