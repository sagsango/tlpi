#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){

  if( argc >= 2 && strcmp(argv[1],"--help") == 0 ){
    printf("uses: $%s pid\n", argv[0]);
    return 0;
  }

  pid_t pid = ( argc >= 2 ? atol(argv[1]) : getpid() );

  for(int i=1; i < argc || (argc == 1 && i == 1); ++i){
    if( argc != 1 )
      pid = atol(argv[i]);
    printf("pid:%ld, priority: %ld\n", (long int) pid, (long int) getpriority(PRIO_PROCESS, getpid()));
  }

  return 0;
}

  

