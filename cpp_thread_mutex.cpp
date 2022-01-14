#include <iostream>
#include <string>
#include <thread>

#define INC 100000

std::thread t[2];
std::mutex t_mtx;

static int gvar = 0;

void f(int x){
	t_mtx.lock();
	for(int i=0;i<x;++i){
		gvar = 1 + gvar;
	}
	t_mtx.unlock();
}

int main(){
	for(int i=0;i<2;++i){
		t[i] = std::thread(f,INC);
	}
	for(int i=0;i<2;++i){
		t[i].join();
	}
	std::cout << "gvar:" << gvar << std::endl; 
}
