#include <stdio.h>
#include <pthread.h>
#define nTHREAD 2

static int g_var = 0;
static int incr = 1000000;

pthread_t t[2];

void* fun(void *ptr){
	int x = *((int*)(ptr));
	for(int i=0;i<x;++i){
		g_var = g_var + 1;
	}
}

int main(){
	for(int i=0;i<nTHREAD;++i){
		pthread_create(&t[i], NULL, &fun, (void*)(&incr) );
	}
	for(int i=0;i<nTHREAD;++i){
		pthread_join(t[i], NULL);
	}
	printf(" g_var = %d\n", g_var);
}
