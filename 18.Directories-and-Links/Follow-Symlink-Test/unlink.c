/*
   rmdir will follow symlink or not ?

   l_dir -> h_dir
   l_dir & h_dir both are empty.
	 

*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

int main(int argc, char * argv[]){

  assert( argc == 2 );

  if( unlink( argv[1] ) != 0 ){

    char e_buf[256];
    memset(e_buf,0,sizeof(e_buf));
    sprintf(e_buf,"unlink() => errno: %d, errtype: %s ", errno, errno == ENOTDIR  ? "ENOTDIR" : "UNKNOWN");
    perror(e_buf);

    return 1;
  }

  return 0;
}


