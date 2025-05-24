/* mmcopy.c

   Copy the contents of one file to another file, using memory mappings.

   Usage mmcopy source-file dest-file
*/
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    if (argc != 3)
        usageErr("%s source-file dest-file\n", argv[0]);

    int fdSrc = open(argv[1], O_RDONLY);
    if (fdSrc == -1)
        errExit("open");

    /* Use fstat() to obtain size of file: we use this to specify the
       size of the two mappings */

    struct stat sb;
    if (fstat(fdSrc, &sb) == -1)
        errExit("fstat");

    /* Handle zero-length file specially, since specifying a size of
       zero to mmap() will fail with the error EINVAL */

    if (sb.st_size == 0)
        exit(EXIT_SUCCESS);

    char *src = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fdSrc, 0);
    if (src == MAP_FAILED)
        errExit("mmap");

    int fdDst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fdDst == -1)
        errExit("open");

    if (ftruncate(fdDst, sb.st_size) == -1)
        errExit("ftruncate");

    char *dst = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE,
                     MAP_SHARED, fdDst, 0);
    if (dst == MAP_FAILED)
        errExit("mmap");

    memcpy(dst, src, sb.st_size);       /* Copy bytes between mappings */

    /* XXX: (Now we know the meaning of MAP_SHARED & MAP_PRIVATE)
     * msync only works if mmaped region is in MAP_SHARED mode
     * MAP_SHARED = mean that mmaped memory is shared between underlying file
     * MAP_PRIVATE = mean that mmaped memory is not shared with the underlying file
     */
    if (msync(dst, sb.st_size, MS_SYNC) == -1)
        errExit("msync");

    exit(EXIT_SUCCESS);
}
 
