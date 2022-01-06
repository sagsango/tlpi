#include <iostream>
#include <thread>
#define MAX_THREAD 2
#define INCREMENT 1000000

std::thread t[MAX_THREAD];
std::mutex mux;
static int num = 0;

void fun(int a){
	mux.lock();
	for(int i=0;i<a;++i){
		num = num + 1;
	}
	mux.unlock();
}

int main(){
	for(int i=0;i<MAX_THREAD;++i){
		t[i] = std::thread(fun,INCREMENT);
	}

	for(int i=0;i<MAX_THREAD;++i){
		//t[i].join();
	}

	std::cout << num << std::endl;
}

