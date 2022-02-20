/*
	 host to network  &  network to host

	 Because port number and other numbers may be having diff representtaion at diff hosts(local machine) - little endian, bigendian.
	 which may be diff from network represenation.

	 we use these function for conversion:-
	 #include <arpa/inet.h>
	 int16_t htons(uint16_t host_uint16); 	//Returns host_uint16 converted to network byte order
	 uint32_t htonl(uint32_t host_uint32); 	//Returns host_uint32 converted to network byte order
	 uint16_t ntohs(uint16_t net_uint16); 	//Returns net_uint16 converted to host byte order
	 uint32_t ntohl(uint32_t net_uint32); 	//Returns net_uint32 converted to host byte order


	 Use of these four functions is necessary only on systems where the host byte order differs from network byte order. 
	 However, these functions should always be used, so that programs are portable to different hardware architectures


	 Output on the macOS Monterey:
	 PORT_NUM = 1234, htons(PORT_NUM) = 53764, ntohs(htons(PORT_NUM)) = 1234

*/

#include <stdio.h>
#include <arpa/inet.h>

#define PORT_NUM 1234
int main(){
	printf(" PORT_NUM = %u, htons(PORT_NUM) = %u, ntohs(htons(PORT_NUM)) = %u\n", PORT_NUM, htons(PORT_NUM), ntohs(htons(PORT_NUM)));
}

