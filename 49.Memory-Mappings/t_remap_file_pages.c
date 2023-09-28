/* t_remap_file_pages.c

   Demonstrate the use of the Linux remap_file_pages() system call to create
   a nonlinear mapping.

   This program is Linux-specific. The remap_file_pages() system call is
   supported since kernel 2.6.
*/
#define _GNU_SOURCE             /* Get remap_file_pages() declaration
                                   from <sys/mman.h> */
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    int fd = open("/tmp/tfile", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    long pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize == -1)
        fatal("Couldn't determine page size");

    for (char ch = 'a'; ch < 'd'; ch++)
        for (int j = 0; j < pageSize; j++)
            write(fd, &ch, 1);

    system("od -a /tmp/tfile");

    char *addr = mmap(NULL, 3 * pageSize, PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");


    addr[pageSize * 0] = 'a';
    addr[pageSize * 1] = 'b';
    addr[pageSize * 2] = 'c';

    printf("Mapped at address %p\n", addr);

    printf("addr[pageSize * 0]:%c\n"
	   "addr[pageSize * 1]:%c\n"
           "addr[pageSize * 2]:%c\n",
           addr[pageSize * 0], addr[pageSize * 1], addr[pageSize * 2]);
/*
  int remap_file_pages(void *addr, size_t size, int prot,
                            size_t pgoff, int flags);

  remap_file_pages - create a nonlinear file mapping

  The pgoff and size arguments specify the region of the file that is  to  be  relo‐
  cated within the mapping: pgoff is a file offset in units of the system page size;
  size is the length of the region in bytes.

  The addr argument serves two purposes.  First, it  identifies  the  mapping  whose
  pages we want to rearrange.  Thus, addr must be an address that falls within a re‐
  gion previously mapped by a call to mmap(2).  Second, addr specifies  the  address
  at which the file pages identified by pgoff and size will be placed.

  The values specified in addr and size should be multiples of the system page size.
  If they are not, then the kernel rounds both values down to the  nearest  multiple
  of the page size.

  The prot argument must be specified as 0.
*/

  /* The three pages of the file -- 0 1 2 -- are currently mapped
       linearly. Now we rearrange the mapping to 2 1 0. */


    if (remap_file_pages(addr, pageSize, 0, 2, 0) == -1)
        errExit("remap_file_pages");

    printf("addr[pageSize * 0]:%c\n"
           "addr[pageSize * 1]:%c\n"
           "addr[pageSize * 2]:%c\n",
           addr[pageSize * 0], addr[pageSize * 1], addr[pageSize * 2]);


    if (remap_file_pages(addr + 2 * pageSize, pageSize, 0, 0, 0) == -1)
        errExit("remap_file_pages");

    printf("addr[pageSize * 0]:%c\n"
           "addr[pageSize * 1]:%c\n"
           "addr[pageSize * 2]:%c\n",
           addr[pageSize * 0], addr[pageSize * 1], addr[pageSize * 2]);

    /* Now we modify the contents of the mapping */

    for (int j = 0; j < 0x100; j++)     /* Modifies page 2 of file */
        *(addr + j) = '0';
    for (int j = 0; j < 0x100; j++)     /* Modifies page 0 of file */
        *(addr + 2 * pageSize + j) = '2';

    system("od -a /tmp/tfile");

    exit(EXIT_SUCCESS);
}
