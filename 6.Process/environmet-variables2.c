#include "tlpi_hdr.h"
int
main(int argc, char *argv[], char *envp[])
{
		char **ep;
    for (ep = envp; *ep != NULL; ep++)
        puts(*ep);
    exit(EXIT_SUCCESS);
}

