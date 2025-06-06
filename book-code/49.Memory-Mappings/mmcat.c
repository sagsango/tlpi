/* mmcat.c

   Use mmap() plus write() to display the contents of a file (specified
   as a command-line argument) on standard output.
*/
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    char *addr;
    int fd;
    struct stat sb;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        errExit("open");

    /* Obtain the size of the file and use it to specify the size of
       the mapping and the size of the buffer to be written */

    /* XXX: stat APIs:
     * stat(const char *restrict file, struct stat *restrict buf)
     * fstat(int fd, struct stat *buf)
     * lstat(const char *restrict file, struct stat *restrict buf)
     */  
    if (fstat(fd, &sb) == -1)
        errExit("fstat");

    /* Handle zero-length file specially, since specifying a size of
       zero to mmap() will fail with the error EINVAL */

    if (sb.st_size == 0)
        exit(EXIT_SUCCESS);

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size)
        fatal("partial/failed write");
    exit(EXIT_SUCCESS);
}
