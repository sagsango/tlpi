/* i6d_ucase_sv.c

   A server that receives datagrams, converts their contents to uppercase, and
   then returns them to the senders.

   See also i6d_ucase_cl.c.
TODO:
  “From the above output, we see that the server’s recvfrom() call was able to 
  obtain the address of the client’s socket, including the ephemeral port number,
  despite the fact that the client did not do a bind().”

  But Kernal did...
  We can say, If one of the client/server used sendto() then other peer should
  have bind() its address already. [Beacuse untill now kernal call have not mede]
*/
#include "i6d_ucase.h"
int
main(int argc, char *argv[])
{
    struct sockaddr_in6 svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];
    char claddrStr[INET6_ADDRSTRLEN];

    /* Create a datagram socket bound to an address in the IPv6 domain */

    sfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sfd == -1)
        errExit("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_addr = in6addr_any;                     /* Wildcard address */
    svaddr.sin6_port = htons(PORT_NUM);
		

		{
			/*
					TODO: see Wildcard address.
					The wildcard is a special local IP address. It usually means "any" and can only be used for bind operations. 
					The value of this IP address is 0.0. 0.0. If you have two network adapters, one with IP address 1.1.
			*/
		
			printf("in6addr_any:");
			for(int i=0;i<16;i+=2){
				printf("%x%x",in6addr_any.s6_addr[i], in6addr_any.s6_addr[i+1]);
				if( i + 2 != 16 ){
					printf(":");
				}
			}
			printf("\n");
		}

		
    if (bind(sfd, (struct sockaddr *) &svaddr,
                sizeof(struct sockaddr_in6)) == -1)
        errExit("bind");

    /* Receive messages, convert to uppercase, and return to client */

    for (;;) {
        len = sizeof(struct sockaddr_in6);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                            (struct sockaddr *) &claddr, &len);
        if (numBytes == -1)
            errExit("recvfrom");

        /* Display address of client that sent the message */

        if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr,
                    INET6_ADDRSTRLEN) == NULL)
            printf("Couldn't convert client address to string\n");
        else
            printf("Server received %ld bytes from (%s, %u)\n",
                    (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
            /*FIXME: above: should use %zd here, and remove (long) cast */

        for (j = 0; j < numBytes; j++)
            buf[j] = toupper((unsigned char) buf[j]);

        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) !=
                numBytes)
            fatal("sendto");
    }
}
