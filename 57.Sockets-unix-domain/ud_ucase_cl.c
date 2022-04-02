/* ud_ucase_cl.c

   A UNIX domain client that communicates with the server in ud_ucase_sv.c.
   This client sends each command-line argument as a datagram to the server,
   and then displays the contents of the server's response datagram.


TODO: Pay attension UDP client socket calles bind().
      Although it is not required see the UDP server-client program in gfg
*/
#include "ud_ucase.h"
int
main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s msg...\n", argv[0]);

    /* Create client socket; bind to unique pathname (based on PID) */

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        errExit("socket");

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_UNIX;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/ud_ucase_cl.%ld", (long) getpid());


    // TODO: In the client we are binding the address of client with client socket.
    //       When binding is necessary? If we want reply back?
    //       No because in TCP server we bind address, but client know the address.
    //       https://stackoverflow.com/questions/39314086/what-does-it-mean-to-bind-a-socket-to-any-address-other-than-localhost
    //       https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/bind.html
    //       see how we use connect() for UDP/Datagram client socket.
    //       After a datagram socket has been connected:
    //       • Datagrams can be sent through the socket using write() (or send()) and are 
    //         automatically sent to the same peer socket. As with sendto(), each write() call 
    //         results in a separate datagram.
    //       • Only datagrams sent by the peer socket may be read on the socket.”
    //       TOCHECK: does connect() call, results binding of ip and port by kernal.

    if (bind(sfd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    /* Construct address of server */

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    /* Send messages to server; echo responses on stdout */

    for (j = 1; j < argc; j++) {
        msgLen = strlen(argv[j]);       /* May be longer than BUF_SIZE */
        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr,
                sizeof(struct sockaddr_un)) != msgLen)
            fatal("sendto");

        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        /* Or equivalently: numBytes = recv(sfd, resp, BUF_SIZE, 0);
                        or: numBytes = read(sfd, resp, BUF_SIZE); */
        if (numBytes == -1)
            errExit("recvfrom");
        printf("Response %d: %.*s\n", j, (int) numBytes, resp);
    }

		sleep(180);
    remove(claddr.sun_path);            /* Remove client socket pathname */
    exit(EXIT_SUCCESS);
}

