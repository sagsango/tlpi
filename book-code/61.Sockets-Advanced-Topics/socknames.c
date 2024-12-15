/* socknames.c

   Demonstrate the use of getsockname() and getpeername() to retrieve the local
   and peer socket addresses.

   Client and server both are here.



   “$ ./socknames 55555 &
   getsockname(connFd):   (localhost, 32835)
   getsockname(acceptFd): (localhost, 55555)
   getpeername(connFd):   (localhost, 55555)
   getpeername(acceptFd): (localhost, 32835)
   [1] 8171$ netstat -a | egrep '(Address|55555)'
   Proto Recv-Q Send-Q Local Address    Foreign Address  State
   tcp        0      0 *:55555          *:*              LISTEN
   tcp        0      0 localhost:32835  localhost:55555  ESTABLISHED
   tcp        0      0 localhost:55555  localhost:32835  ESTABLISHED”

*/
#include "inet_sockets.h"               /* Declares our socket functions */
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
    int listenFd, acceptFd, connFd;
    socklen_t len;                      /* Size of socket address buffer */
    void *addr;                         /* Buffer for socket address */
    char addrStr[IS_ADDR_STR_LEN];

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s service\n", argv[0]);

    /* Create listening socket, obtain size of address structure */

    listenFd = inetListen(argv[1], 5, &len);
    if (listenFd == -1)
        errExit("inetListen");

    connFd = inetConnect(NULL, argv[1], SOCK_STREAM);
    if (connFd == -1)
        errExit("inetConnect");

    acceptFd = accept(listenFd, NULL, NULL);
    if (acceptFd == -1)
        errExit("accept");

    addr = malloc(len);
    if (addr == NULL)
        errExit("malloc");

    if (getsockname(connFd, addr, &len) == -1)
        errExit("getsockname");
    printf("getsockname(connFd):   %s\n",
            inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

    if (getsockname(acceptFd, addr, &len) == -1)
        errExit("getsockname");
    printf("getsockname(acceptFd): %s\n",
            inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

    if (getpeername(connFd, addr, &len) == -1)
        errExit("getpeername");
    printf("getpeername(connFd):   %s\n",
            inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

    if (getpeername(acceptFd, addr, &len) == -1)
        errExit("getpeername");
    printf("getpeername(acceptFd): %s\n",
            inetAddressStr(addr, len, addrStr, IS_ADDR_STR_LEN));

    sleep(30);                          /* Give us time to run netstat(8) */
    exit(EXIT_SUCCESS);
}
