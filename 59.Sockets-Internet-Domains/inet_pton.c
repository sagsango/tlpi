/*
	 Converting IPv4 and IPv6 addresses between binary and human-readable forms
	 The inet_pton() and inet_ntop() functions are like inet_aton() and inet_ntoa(), 
	 but differ in that they also handle IPv6 addresses. They convert binary IPv4 and 
	 IPv6 addresses to and from presentation format—that is, either dotted-decimal 
	 or hex-string notation.


	 $ ./inet_pton i6   0:0:0:0:0:0:0:0 
	 $ ./inet_pton i6   1:0:0:0:0:0:0:8
	 $ ./inet_pton i6   0:0:0:0:0:FFFF:204.152.189.116
	 $ ./inet_pton i4   126.178.32.1
*/

#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

int main(int argc, char *argv[]) {
	unsigned char buf[sizeof(struct in6_addr)];
	int domain, s;
	char str[INET6_ADDRSTRLEN];

	if (argc != 3) {
		fprintf(stderr, "Usage: %s {i4|i6|<num>} string\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	domain = (strcmp(argv[1], "i4") == 0) ? AF_INET :
		(strcmp(argv[1], "i6") == 0) ? AF_INET6 : atoi(argv[1]);

	s = inet_pton(domain, argv[2], buf);
	if (s <= 0) {
		if (s == 0)
			fprintf(stderr, "Not in presentation format");
		else
			perror("inet_pton");
		exit(EXIT_FAILURE);
	}

	if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
		perror("inet_ntop");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", str);

	exit(EXIT_SUCCESS); 
}
