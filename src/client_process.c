#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client_funcs.h"
#include "sizes.h"

int main()
{
    int sockfd, len, result, i, nread;
    struct sockaddr_in address; /* Client socket address structure */
    char filename[MAX_NAME_SZ]; /* Name of file to request from server */
    char resp_from_server[MAX_MSG_SZ]; /* Contents of served file*/

    /* Create socket for client */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror(__FILE__ " socket()");
        exit(1);
    }
    /* Name the socket */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    len = sizeof(address);

    /* Call connect() to establish connection with server listen socket */
    if ((result = connect(sockfd, (struct sockaddr *)&address, len)) == -1)
    {
        perror(__FILE__ " connect()");
        exit(1);
    }
    /* Get names of available files to be served */
    puts("[CLIENT] connection established with server, choose from one of the "
         "following pages:");
    for (i = 0; i < MAX_STATIC_FILES; ++i)
    {
        char contents[MAX_NAME_SZ];
        memset(contents, '\0', sizeof(contents));
        read(sockfd, &contents, MAX_MSG_SZ); /* Read contents of static dir */
        if (contents[0] != '\0')
            printf("> %s\n", contents);
    }
    
    /* Input name of file that client wants to request */
    cli_request(filename);
    write(sockfd, &filename, sizeof(filename)); /* Pass filename to server */

    /* Read response from server and print to console */
    nread = read(sockfd, &resp_from_server, sizeof(resp_from_server));
    if (nread)
        printf("[CLIENT] response from server:\n%s\n", resp_from_server);

    close(sockfd);
    exit(0);
}
