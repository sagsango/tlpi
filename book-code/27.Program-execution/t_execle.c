/* t_execle.c

   Demonstrate the use of execle() to execute a program.
*/
#include "tlpi_hdr.h"

/*
27.Program-execution ❱❱❱ ./t_execle /usr/bin/ls
ls: cannot access 'hello world': No such file or directory
ls: cannot access 'goodbye': No such file or directory
27.Program-execution ❱❱❱ ./t_execle /usr/bin/echo
hello world goodbye
27.Program-execution ❱❱❱
*/
int
main(int argc, char *argv[])
{
    char *envVec[] = { "GREET=salut", "BYE=adieu", NULL };
    char *filename;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    /* Execute the program specified in argv[1] */

    filename = strrchr(argv[1], '/');       /* Get basename from argv[1] */
    if (filename != NULL)
        filename++;
    else
        filename = argv[1];

    execle(argv[1], filename, "hello world", "goodbye", (char *) NULL, envVec);
    errExit("execle");          /* If we get here, something went wrong */
}
