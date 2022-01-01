#include <stdio.h>
#include <unistd.h>

int main(){
	printf("Process id:%d, Parent process id:%d\n",
			getpid(),getppid());
}
