#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>


int main(void)
{
	struct addrinfo *result;
	struct addrinfo *res;
	/*
		 struct addrinfo {
		     int       ai_flags;         // Input flags (AI_* constants) 
		     int       ai_family;        // Address family 
		     int       ai_socktype;      // Type: SOCK_STREAM, SOCK_DGRAM 
		     int       ai_protocol;      // Socket protocol 
		     socklen_t ai_addrlen;       // Size of structure pointed to by ai_addr 
		     char     *ai_canonname;     // Canonical name of host 
		     struct sockaddr *ai_addr;   // Pointer to socket address structure 
		     struct addrinfo *ai_next;   // Next structure in linked list 
		 };
	 */
    int error;
 
 
    /* resolve the domain name into a list of addresses */
    error = getaddrinfo("www.freebsd.org", NULL, NULL, &result);
    if (error != 0)
    {   
        fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
        return EXIT_FAILURE;
    }   
 
 
    /* loop over all returned results and do inverse lookup */
    for (res = result; res != NULL; res = res->ai_next)
    {   
        char hostname[NI_MAXHOST] = "";
 
 
        error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0); 
        if (error != 0)
        {
            fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
            continue;
        }
        if (*hostname != '\0')
            printf("hostname: %s\n", hostname);
    }   
 
 
    freeaddrinfo(result);
    return EXIT_SUCCESS;
}
