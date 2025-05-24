#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("Process id:%d, Parent process id:%d\n",
		getpid(),getppid());

		// clearenv()
        // getenv(const char *name);
        // const char *value, int replace)
        // putenv(char *string) "name=val"
        // unsetenv(const char *name)

}
