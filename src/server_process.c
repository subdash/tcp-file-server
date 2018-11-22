#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server_funcs.h"
#include "sizes.h"

int main()
{
    char** files = static_file_tree();
    int server_sockfd, client_sockfd, server_len, client_len;
    struct sockaddr_in server_address, client_address;
    fd_set readfds, testfds;

    /* Create an unnamed socket */
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        perror(__FILE__ " socket()");

    /* Name the socket */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    server_len = sizeof(server_address);
    if (bind(server_sockfd, (struct sockaddr *)&server_address,
        server_len) == -1)
    {
        perror(__FILE__ " bind()");
        exit(1);
    }
    /* Create connection queue and wait for clients */
    if ((listen(server_sockfd, BACKLOG)) == -1)
        perror(__FILE__ " listen()");

    /* Initialize readfds as empty fd_set and add server_sockfd to it */
    FD_ZERO(&readfds); 
    FD_SET(server_sockfd, &readfds);

    puts("[SERVER] waiting...");
    while (1)
    {
        char file_requested[MAX_MSG_SZ];
        int fd;
        int nread;
        testfds = readfds;
        
        /* select() tests whether any fd of the fd_sets is ready for reading,
         * writing, or has pending error condition, and blocks until one of
         * those conditions is met. */
        if ((select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0,
            (struct timeval *)0)) == -1)
        {
            perror(__FILE__ " select()");
            exit(1);
        }
        /* Check for activity on each file descriptor in testfds */
        for (fd = 0; fd < FD_SETSIZE; fd++)
        {
            /* If activity is on server_sockfd, it must be a request for a new
             * connection. */
            if (FD_ISSET(fd, &testfds) && (fd == server_sockfd))
            {
                client_len = sizeof(client_address);
                if ((client_sockfd = accept(server_sockfd,
                    (struct sockaddr *)&client_address,
                    (socklen_t *)&client_len)) == -1)
                    perror(__FILE__ " accept()");
                /* Add the associated client_sockfd to the descriptor set. */
                FD_SET(client_sockfd, &readfds);
                printf("[SERVER] adding client on fd %d\n", client_sockfd);
                /* Tell client what files are available */
                greet_client(files, client_sockfd);
            }
            /* If it isn't the server, it's client activity. */
            else if (FD_ISSET(fd, &testfds))
            {
                ioctl(fd, FIONREAD, &nread);
                /* If close is received, the client has gone away, so it can be
                 * removed from the descriptor set. */
                if (nread == 0)
                {
                    close(fd);
                    FD_CLR(fd, &readfds);
                    printf("[SERVER] removed client on fd %d\n", fd);
                }
                /* Serve the client. */
                else
                {
                    int valid;
                    read(fd, &file_requested, MAX_NAME_SZ);
                    printf("[SERVER] client requested %s on fd %d\n",
                        file_requested, fd);
                    valid = match(file_requested, files);
                    serve_client(file_requested, fd, valid);
                }
            }
        }
    }
}
