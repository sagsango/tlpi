/*
   rmdir will follow symlink or not ?

   l_dir -> h_dir
   l_dir & h_dir both are empty.
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


int main(int argc, char * argv[]){

  assert( argc == 3 );


  if( rename( argv[1], argv[2] ) != 0 ){
    perror("rename():");
    return 1;
  }


  return 0;
}


