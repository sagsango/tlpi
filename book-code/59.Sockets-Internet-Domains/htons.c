#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

int main(int argc,char *argv[])
{
		struct servent * t_ser;
		/* struct servent {
               char  *s_name;       // official service name 
               char **s_aliases;    // alias list 
               int    s_port;       // port number 
               char  *s_proto;      // protocol to use  
		}	*/


		const char * serv_name = argc >= 2 ? argv[1] : "ssh";
		if((t_ser = getservbyname(serv_name, NULL /*"tcp" or "udp" or NULL */)) == NULL ){
			fprintf(stderr,"Error: getservebyname()\n");
			return 1;
		}

		printf("s_name: %s\ns_port:	%u\ns_proto: %s\n",	t_ser->s_name, ntohs(t_ser->s_port), t_ser->s_proto);
		for(char **p = t_ser->s_aliases; *p != NULL; ++p){
			printf("alias:	%s\n", *p);
		}


		int port_number = argc >= 3 ? atoi(argv[2]) : 20;
		if((t_ser = getservbyport(htons(port_number), "tcp"/*"tcp" or "udp" or NULL*/)) == NULL ){
			fprintf(stderr,"Error: getservebyport()\n");
			return 1;
		}

		printf("s_name: %s\ns_port:	%u\ns_proto: %s\n",	t_ser->s_name, ntohs(t_ser->s_port), t_ser->s_proto);
		for(char **p = t_ser->s_aliases; *p != NULL; ++p){
			printf("alias:	%s\n", *p);
		}

		return 0;
}

