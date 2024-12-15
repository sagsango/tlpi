/* t_fpathconf.c

   Demonstrate the use of fpathconf() to retrieve the values of
   pathname-related limits.
*/
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define TMP_FILE "t_fpathconf.txt"

static void             /* Print 'msg' plus value of fpathconf(fd, name) */
fpathconfPrint(const char *msg, int fd, int name)
{
    long lim;

    errno = 0;
  
		lim = fpathconf(fd, name);
	  if (lim != -1) {        /* Call succeeded, limit determinate */
        printf("%s %ld\n", msg, lim);
    } else {
        if (errno == 0)     /* Call succeeded, limit indeterminate */
            printf("%s (indeterminate)\n", msg);
        else                /* Call failed */
            errExit("fpathconf %s", msg);
    }
}
int
main(int argc, char *argv[])
{
		int fd;

#ifdef __APPLE__
		fd = open( TMP_FILE, O_CREAT | O_RDWR);
		if( fd == -1 ){
			perror("open(" TMP_FILE "):");
			return 1;
		}
#else
		fd = STDIN_FILENO;
#endif

    fpathconfPrint("_PC_NAME_MAX: ", fd, _PC_NAME_MAX);
    fpathconfPrint("_PC_PATH_MAX: ", fd, _PC_PATH_MAX);
    fpathconfPrint("_PC_PIPE_BUF: ", fd, _PC_PIPE_BUF);

#ifdef __APPLE__
		close(fd);
		unlink(TMP_FILE);
#endif

    exit(EXIT_SUCCESS);
}
