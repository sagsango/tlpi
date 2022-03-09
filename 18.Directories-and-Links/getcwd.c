#include <stdio.h>
#include <unistd.h>

static char buf[512];
int main(){
	if( getcwd(buf, 512) == NULL ){
		perror("getcwd()");
		return 1;
	}
	printf("cwd:%s\n",buf);
	return 0;
}

