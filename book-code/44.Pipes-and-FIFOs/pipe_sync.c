/* pipe_sync.c

   Show how pipes can be used for synchronizing the actions of a parent and
   multiple child processes.

   Usage: pipe_sync sleep-time...

   After creating a pipe, the program creates one child for each command-line
   argument. Each child simulates doing some work by sleeping for the number of
   seconds specified in the corresponding command-line argument. When it has
   finished doing its "work", each child closes its file descriptor for the
   write end of the pipe; the parent can see that all children have finished
   their work when it sees end-of-file on the read end of the pipe.
*/


/*
44.Pipes-and-FIFOs ❱❱❱ ./pipe_sync  1 1 1 1
05:45:07  Parent started
Parent Reads:C
Parent Reads:h
Parent Reads:i
Parent Reads:l
Parent Reads:d
Parent Reads:
Parent Reads:1
Parent Reads:
Parent Reads:(
Parent Reads:P
Parent Reads:I
Parent Reads:D
Parent Reads:=
Parent Reads:1
Parent Reads:0
Parent Reads:5
Parent Reads:8
Parent Reads:0
Parent Reads:6
Parent Reads:0
Parent Reads:)
Parent Reads:

Parent Reads:C
Parent Reads:h
Parent Reads:i
Parent Reads:l
Parent Reads:d
Parent Reads:
Parent Reads:2
Parent Reads:
Parent Reads:(
Parent Reads:P
Parent Reads:I
Parent Reads:D
Parent Reads:=
Parent Reads:1
Parent Reads:0
Parent Reads:5
Parent Reads:8
Parent Reads:0
Parent Reads:6
Parent Reads:1
Parent Reads:)
Parent Reads:

Parent Reads:C
Parent Reads:h
Parent Reads:i
Parent Reads:l
Parent Reads:d
Parent Reads:
Parent Reads:3
Parent Reads:
Parent Reads:(
Parent Reads:P
Parent Reads:I
Parent Reads:D
Parent Reads:=
Parent Reads:1
Parent Reads:0
Parent Reads:5
Parent Reads:8
Parent Reads:0
Parent Reads:6
Parent Reads:2
Parent Reads:)
Parent Reads:

Parent Reads:C
Parent Reads:h
Parent Reads:i
Parent Reads:l
Parent Reads:d
Parent Reads:
Parent Reads:4
Parent Reads:
Parent Reads:(
Parent Reads:P
Parent Reads:I
Parent Reads:D
Parent Reads:=
Parent Reads:1
Parent Reads:0
Parent Reads:5
Parent Reads:8
Parent Reads:0
Parent Reads:6
Parent Reads:3
Parent Reads:)
Parent Reads:

05:45:08  Parent ready to go
*/

#include "curr_time.h"                      /* Declaration of currTime() */
#include "tlpi_hdr.h"

#define SIZE 1000
#define READ_BYTES 1 // TODO: change read bytes.
int
main(int argc, char *argv[])
{
    int pfd[2];                             /* Process synchronization pipe */
    int j, dummy;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sleep-time...\n", argv[0]);

    setbuf(stdout, NULL);                   /* Make stdout unbuffered, since we
                                               terminate child with _exit() */
    printf("%s  Parent started\n", currTime("%T"));

    if (pipe(pfd) == -1)
        errExit("pipe");

    for (j = 1; j < argc; j++) {
        switch (fork()) {
        case -1:
            errExit("fork %d", j);

        case 0: /* Child */
            if (close(pfd[0]) == -1)        /* Read end is unused */
                errExit("close");

            /* Child does some work, and lets parent know it's done */

            sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));
                                            /* Simulate processing */
            /*printf("%s  Child %d (PID=%ld) closing pipe\n",
                    currTime("%T"), j, (long) getpid());*/

						char msg[SIZE];
						memset(msg,0,sizeof(msg));
						sprintf(msg,"Child %d (PID=%ld)\n", j, (long) getpid());
						if( write(pfd[1], msg, strlen(msg)) != strlen(msg) ){
							errExit("Write: %d\n",j);
						}

/*
TODO: See evey child can close their discripter.
    Here (write end of the pipe) Fd is beining shared by multiple processes.
*/
                
            if (close(pfd[1]) == -1)
                errExit("close");
            /* Child now carries on to do other things... */

            _exit(EXIT_SUCCESS);

        default: /* Parent loops to create next child */
            break;
        }
    }

    /* Parent comes here; close write end of pipe so we can see EOF */

    if (close(pfd[1]) == -1)                /* Write end is unused */
        errExit("close");

    /* Parent may do other work, then synchronizes with children */

		char buf[SIZE];
		memset(buf,0,sizeof(buf));
    /* XXX: Parent is going to read msg by the multiple writer
            Through the same write end of the pipe

            And when all the write end of the pipe get closed.
            Then only read end will get the 0 bye on read.
    */
    while( read(pfd[0], &buf, READ_BYTES) != 0 ){
        //fatal("parent didn't get EOF");

		    printf("Parent Reads:%s\n", buf);
        //printf("%s", buf);
        memset(buf,0,sizeof(buf));

		}
    printf("%s  Parent ready to go\n", currTime("%T"));

    /* Parent can now carry on to do other things... */

    /*
     *
     * NOTE: Parent did not do wait
     *       But still we are sure that all the pipe read end have been closed
     *       So from the children side there is not much going on
     *       But to be safe we should do wait() here
     */

    exit(EXIT_SUCCESS);
}
