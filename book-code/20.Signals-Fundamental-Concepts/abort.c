#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


void fun(int *a, int *b){
	abort();
}
int main(){
	int a = 10;
	int b = 10;
	fun(&a,&b);
	return 0;
}
