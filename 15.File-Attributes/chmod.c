#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FILE "./chmod.txt"
int main(){
	int fd = open( FILE, O_CREAT | O_APPEND, (S_IRWXU) | (0) | (0) );
	if( fd == -1 ){
		perror("open():");
		return 1;
	}
#if defined ( __APPLE__ )
	char cmd[256] = "stat -x ";
#elif defined ( __linux__ )
	char cmd[256] = "stat ";
#else
	char cmd[256] = "stat ";
#endif
	strcat(cmd, FILE);

	system(cmd);

	if( chmod( FILE, S_ISVTX | (S_IRUSR) | (S_IRGRP) | (S_IROTH) ) ){
		perror("chmod():");
		return 2;
	}

	system(cmd);

 	if( chmod( FILE, 0 |  (S_IRUSR) | (S_IRGRP) | (S_IROTH) ) ){
                perror("chmod():");
                return 3;
        }	

	system(cmd);

	if( unlink(FILE) ){
		perror("unlink():");
		return 4;
	}

	return 0;
}


