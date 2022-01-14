/* t_statfs.c

	 Demonstrate the use of statfs() to retrieve information about
	 a mounted file system.

	 This program is Linux-specific.

	 See also t_statvfs.c.
 */
#if defined(__APPLE__)
#include <sys/param.h>
#include <sys/mount.h>
#else
#include <sys/statfs.h>
#endif
#include "tlpi_hdr.h"
	int
main(int argc, char *argv[])
{
	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s path\n", argv[0]);

	struct statfs sfs;
	if (statfs(argv[1], &sfs) == -1)
		errExit("statfs");

	printf("File system type:              %#lx\n",
			(unsigned long) sfs.f_type);
	printf("Optimal I/O block size:        %lu\n",
			(unsigned long) sfs.f_bsize);
	printf("Total data blocks:             %lu\n",
			(unsigned long) sfs.f_blocks);
	printf("Free data blocks:              %lu\n",
			(unsigned long) sfs.f_bfree);
	printf("Free blocks for nonsuperuser:  %lu\n",
			(unsigned long) sfs.f_bavail);
	printf("Total i-nodes:                 %lu\n",
			(unsigned long) sfs.f_files);
#if defined(__APPLE__)
	printf("File system ID:                %#x, %#x\n",
			(unsigned) sfs.f_fsid.val[0], (unsigned) sfs.f_fsid.val[1]);
#else
	printf("File system ID:                %#x, %#x\n",
			(unsigned) sfs.f_fsid.__val[0], (unsigned) sfs.f_fsid.__val[1]);
#endif
	printf("Free i-nodes:                  %lu\n",
			(unsigned long) sfs.f_ffree);
#if defined(__APPLE__)
	printf("Maximum file name length:      %lu\n",
			(unsigned long) MAXPATHLEN);
#else
	printf("Maximum file name length:      %lu\n",
			(unsigned long) sfs.f_namelen);
#endif
	exit(EXIT_SUCCESS);
}
