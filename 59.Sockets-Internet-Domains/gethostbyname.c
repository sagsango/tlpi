#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>


/*
struct hostent *gethostbyname(const char *name);
struct hostent *gethostbyaddr(const char *addr, int len, int type);
*/


int main(int argc, char *argv[]) {
	int i;
	struct hostent *he;
	struct in_addr **addr_list;
	struct in_addr addr;

	// get the addresses of www.yahoo.com:

	const char * domain_name = argc >= 2 ? argv[1] : "yahoo.com";
	he = gethostbyname(domain_name);
	if (he == NULL) { // do some error checking
		herror("gethostbyname"); // herror(), NOT perror()
		return 1;
	}

	// print information about this host:
	printf("Official name is: %s\n", he->h_name);
	printf("IP address: %s\n", inet_ntoa(*(struct in_addr*)he->h_addr));
	printf("All addresses: ");
	addr_list = (struct in_addr **)he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++) {
		printf("%s ", inet_ntoa(*addr_list[i]));
	}
	printf("\n");

	// get the host name of 157.240.16.35
	const char * ip4 = argc >= 3 ? argv[2] : "157.240.16.35"; // TODO: Put approriate IPv4
	if( inet_aton(ip4, &addr) == 0 ){
		return 0;
	}

	he = gethostbyaddr(&addr, sizeof(addr), PF_INET);

	printf("Host name: %s\n", he->h_name);
}
