/*
	Converting IPv4 addresses between binary and human-readable forms
	The inet_aton() and inet_ntoa() functions convert an IPv4 address 
	in dotted-decimal notation to binary and vice versa. 

	$ ./inet_aton 226.000.000.037
*/
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char *argv[]) {
	struct in_addr addr;
	/*
		 struct in_addr {                 // IPv4 4-byte address 
    		in_addr_t s_addr;             // Unsigned 32-bit integer 
		 };
	*/

	if (argc != 2) {
		fprintf(stderr, "%s <dotted-address>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (inet_aton(argv[1], &addr) == 0) {
		fprintf(stderr, "Invalid address\n");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", inet_ntoa(addr));
	printf("addr.s_addr:%u\n", addr.s_addr);

	exit(EXIT_SUCCESS); 
}
