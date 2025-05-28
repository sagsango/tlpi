/* # Uses:
   Copy the file named argv[1] to a new file named in argv[2].

	 $ ./copy test test.old           Copy a regular file
	 $ ./copy a.txt /dev/tty          Copy a regular file to this terminal
	 $ ./copy /dev/tty b.txt          Copy input from this terminal to a regular file
		EOF (End Of File) character, which is Ctrl+D; for terminal
	 $ ./copy /dev/pts/16 /dev/tty    Copy input from another terminal


*/

/* XXX:
 * The /dev/tty device file in Linux is a special file that
 * refers to the controlling terminal of the current process.
 * When a process interacts with /dev/tty, it communicates
 * with the terminal associated with its session, regardless
 * of whether it’s a physical terminal, pseudo-terminal
 * (e.g., in a terminal emulator like xterm or gnome-terminal),
 * or another type of terminal device.
 */

/*
 * XXX:
 *  How many controling terminals are there?
 *  These are pseudo terminals.

04.File-IO-The-Universal-IO-Model ❱❱❱ ls /dev/tty*
/dev/tty    /dev/tty14  /dev/tty20  /dev/tty27  /dev/tty33  /dev/tty4   /dev/tty46  /dev/tty52  /dev/tty59  /dev/tty8       /dev/ttyS12  /dev/ttyS19  /dev/ttyS25  /dev/ttyS31
/dev/tty0   /dev/tty15  /dev/tty21  /dev/tty28  /dev/tty34  /dev/tty40  /dev/tty47  /dev/tty53  /dev/tty6   /dev/tty9       /dev/ttyS13  /dev/ttyS2   /dev/ttyS26  /dev/ttyS4
/dev/tty1   /dev/tty16  /dev/tty22  /dev/tty29  /dev/tty35  /dev/tty41  /dev/tty48  /dev/tty54  /dev/tty60  /dev/ttyprintk  /dev/ttyS14  /dev/ttyS20  /dev/ttyS27  /dev/ttyS5
/dev/tty10  /dev/tty17  /dev/tty23  /dev/tty3   /dev/tty36  /dev/tty42  /dev/tty49  /dev/tty55  /dev/tty61  /dev/ttyS0      /dev/ttyS15  /dev/ttyS21  /dev/ttyS28  /dev/ttyS6
/dev/tty11  /dev/tty18  /dev/tty24  /dev/tty30  /dev/tty37  /dev/tty43  /dev/tty5   /dev/tty56  /dev/tty62  /dev/ttyS1      /dev/ttyS16  /dev/ttyS22  /dev/ttyS29  /dev/ttyS7
/dev/tty12  /dev/tty19  /dev/tty25  /dev/tty31  /dev/tty38  /dev/tty44  /dev/tty50  /dev/tty57  /dev/tty63  /dev/ttyS10     /dev/ttyS17  /dev/ttyS23  /dev/ttyS3   /dev/ttyS8
/dev/tty13  /dev/tty2   /dev/tty26  /dev/tty32  /dev/tty39  /dev/tty45  /dev/tty51  /dev/tty58  /dev/tty7   /dev/ttyS11     /dev/ttyS18  /dev/ttyS24  /dev/ttyS30  /dev/ttyS9

 *
 *
 *
 */



#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif
int
main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);

    /* Open input and output files */

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[1]);

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1)
        errExit("opening file %s", argv[2]);

    /* Transfer data until we encounter end of input or an error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            fatal("write() returned error or partial write occurred");
    if (numRead == -1)
        errExit("read");

    if (close(inputFd) == -1)
        errExit("close input");
    if (close(outputFd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}
