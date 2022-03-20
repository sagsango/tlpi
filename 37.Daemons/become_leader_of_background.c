#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int                                     /* Returns 0 on success, -1 on error */
becomeLeaderOfBackground()
{
    int maxfd, fd;

    switch (fork()) {                   /* Become background process */
    case -1: return -1;
    case 0:  break;                     /* Child falls through... */
    default: _exit(EXIT_SUCCESS);       /* while parent terminates */
                                        /* child become orphan */
                                        /* get adompted by init */
    }

    if (setsid() == -1)                 /* Become leader of new session */
        return -1;

    return 0;
}

int main(){
  if( becomeLeaderOfBackground() ){
    perror("becomeLeaderOfBackground()");
    exit(EXIT_FAILURE);
  }
  // be alive for some time.
  sleep(100);
}
