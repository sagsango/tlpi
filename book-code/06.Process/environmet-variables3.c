
/* display_env.c

   Display the process environment list.
*/
#include "tlpi_hdr.h"
#include <stdlib.h>
int
main(int argc, char *argv[])
{
    const char * name = "HOME";
		const char * val = getenv(name);
		puts(val);

    exit(EXIT_SUCCESS);
    /*
        setenv(const char *name, const char *value, int replace);
        putenv(char *string); // "name=value"
    */
}
