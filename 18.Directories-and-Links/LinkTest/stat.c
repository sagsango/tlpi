
/*
	 stat()  and  fstatat()  retrieve  information  about the file pointed to by pathname; 
	 the differences for fstatat() are described below.

   lstat() is identical to stat(), except that if pathname is a symbolic link, then it 
	 returns information about the link itself, not the file that it refers to.

   fstat()  is identical to stat(), except that the file about which information is to 
	 be retrieved is specified by the file descriptor fd.



	TODO:
		1. stat on symlink, get dereferanced, means will give stat about link, rather thann file itself.
		2. lstat will not dereferance the link, so it will always give you curr file stat.
		3. It is always better to use lstat if you want to know the info about file.
*/


#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#else
#include <sys/sysmacros.h>
#endif // __APPLE__


	int
main(int argc, char *argv[])
{
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &sb) == -1) {
		perror("lstat");
		exit(EXIT_FAILURE);
	}

	printf("ID of containing device:  [%lx,%lx]\n",
			(long) major(sb.st_dev), (long) minor(sb.st_dev));

	printf("File type:                ");
	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}

	printf("I-node number:            %ld\n", (long) sb.st_ino);

	printf("Mode:                     %lo (octal)\n",
			(unsigned long) sb.st_mode);

	printf("Link count:               %ld\n", (long) sb.st_nlink);
	printf("Ownership:                UID=%ld   GID=%ld\n",
			(long) sb.st_uid, (long) sb.st_gid);

	printf("Preferred I/O block size: %ld bytes\n",
			(long) sb.st_blksize);
	printf("File size:                %lld bytes\n",
			(long long) sb.st_size);
	printf("Blocks allocated:         %lld\n",
			(long long) sb.st_blocks);

	printf("Last status change:       %s", ctime(&sb.st_ctime));
	printf("Last file access:         %s", ctime(&sb.st_atime));
	printf("Last file modification:   %s", ctime(&sb.st_mtime));

	exit(EXIT_SUCCESS);
}

