/* t_execlp.c

   Demonstrate the use of execlp() to execute a program.

   And the file will be search in the all the paths,
   mentioned in PATH environment variable.


  $ which echo
    /bin/echo
  $ ls -l /bin/echo
    -rwxr-xr-x    1 root      15428 Mar 19 21:28 /bin/echo
  $ echo $PATH                     //Show contents of PATH environment variable
    /home/mtk/bin:/usr/local/bin:/usr/bin:/bin          // /bin is in PATH
  $./t_execlp echo                 //execlp() uses PATH to successfully find echo
    hello world

  $ PATH=/home/mtk/bin:/usr/local/bin:/usr/bin
  $ ./t_execlp echo
    ERROR [ENOENT No such file or directory] execlp
  $ ./t_execlp /bin/echo
    hello world
*/

#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    /* Execute the program specified in argv[1] */

    execlp(argv[1], argv[1], "hello world", (char *) NULL);
    errExit("execlp");      /* If we get here, something went wrong */
}
