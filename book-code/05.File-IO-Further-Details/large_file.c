/* large_file.c

   Demonstrate the use of the (obsolete) Large File System API.

   This program is Linux-specific.
*/
#define _LARGEFILE64_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifdef __APPLE__
#define off64_t off_t
#define open64 open
#define lseek64 lseek
#endif

int
main(int argc, char *argv[])
{
    int fd;
    off64_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname offset\n", argv[0]);

    /*
     *  XXX: Why we need open64?
     *       Because file struct will be having offset data member
     *       and it will be int32_t by default.
     *
     *       We want to use the 64 bit int64_t
     *
     *      open64 = open with O_LARGEFILE flag
     * 
     */
    fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open64");

    off = atoll(argv[2]);
    if (lseek64(fd, off, SEEK_SET) == -1)
        errExit("lseek64");

    if (write(fd, "test", 4) == -1)
        errExit("write");
    exit(EXIT_SUCCESS);
}
