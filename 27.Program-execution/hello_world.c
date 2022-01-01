#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[], char *envV[]){
	printf("Process Id: %ld, Parent Id: %ld\n", (long) getpid(), (long) getppid());
	printf("%s\n", argv[1]);
	printf("Exicute....\n");
	printf("%s\n", argv[2]);
	return 0;
}
