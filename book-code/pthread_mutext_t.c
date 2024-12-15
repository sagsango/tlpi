#include <stdio.h>
#include <pthread.h>
#define nTHREAD 2

static int g_var = 0;
static int incr = 1000000;

pthread_t t[2];
pthread_attr_t t_attr[2];

pthread_mutex_t mutex;

void* fun(void *ptr){
	int x = *((int*)(ptr));
	pthread_mutex_lock(&mutex);
	for(int i=0;i<x;++i){
		g_var = g_var + 1;
	}
	pthread_mutex_unlock(&mutex);
}

int main(){
	pthread_mutex_init(&mutex,NULL);
	for(int i=0;i<nTHREAD;++i){
		pthread_attr_init(&t_attr[i]);
		pthread_create(&t[i], &t_attr[i], &fun, (void*)(&incr) );
		pthread_attr_destroy(&t_attr[i]);
	}
	for(int i=0;i<nTHREAD;++i){
		pthread_join(t[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	printf(" g_var = %d\n", g_var);
}
