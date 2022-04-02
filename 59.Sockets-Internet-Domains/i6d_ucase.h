/* i6d_ucase.h

   Header file for i6d_ucase_sv.c and i6d_ucase_cl.c.
*/
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 10                     /* Maximum size of messages exchanged
                                           between client and server */

#define PORT_NUM 50002                  /* Server port number */
/*
TODO:
  Server Post number is fixed, now run the program with diff client
  see how client port number get assigned automatically and will
  differ for different client.
  
  This automatic assinment is done by the kernal.
*/
