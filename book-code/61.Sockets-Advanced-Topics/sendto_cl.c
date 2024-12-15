/* https://stackoverflow.com/questions/11952898/c-send-and-receive-file */
/* see sendto_sv.c also */
/* TODO : man sendto() */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT_NUMBER     5990
#define SERVER_ADDRESS  /*"192.168.1.7"*/ "127.0.0.1" //loopback
#define FILENAME        "./sendto_cl.txt"

int main(int argc, char **argv)
{
        int client_socket;
        ssize_t len;
        struct sockaddr_in remote_addr;
        char buffer[BUFSIZ];
        int file_size;
        FILE *received_file;
        int remain_data = 0;

        /* Zeroing remote_addr struct */
        memset(&remote_addr, 0, sizeof(remote_addr));

        /* Construct remote_addr struct */
        remote_addr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_ADDRESS, &(remote_addr.sin_addr));
        remote_addr.sin_port = htons(PORT_NUMBER);

        /* Create client socket */
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
                fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Connect to the server */
        if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
        {
                fprintf(stderr, "Error on connect --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Receiving file size */
        recv(client_socket, buffer, BUFSIZ, 0);
        file_size = atoi(buffer);
        fprintf(stdout, "\nFile size : %d\n", file_size);


        received_file = fopen(FILENAME, "w");
        if (received_file == NULL)
        {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        remain_data = file_size;

        while (((len = recv(client_socket, buffer, BUFSIZ, 0)) > 0))
        {
                fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                fprintf(stdout, "Receive %d bytes and total remaining data:- %d bytes\n", len, remain_data);
        }
        printf("last receive => len:%d, remaning data:%d\n", len, remain_data);
        fclose(received_file);

        close(client_socket);

        return 0;
}
