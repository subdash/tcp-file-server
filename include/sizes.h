#ifndef NET_UTILS_H
#define NET_UTILS_H

/* This header file defines some standard limits for file I/O as well as the
 * port number to open a socket on.
 */

#define BACKLOG          5  /* Max # of connections server will keep in queue */
#define MAX_STATIC_FILES 5  /* Max # of files in /static directory */
#define MAX_NAME_SZ     32  /* Max length of filename client can request */
#define MSG_CHUNK_SZ    64  /* Server sends data in chunks of this size */
#define MAX_MSG_SZ    1024  /* Max # of bytes server will send to client */
#define PORT          9734  /* Port that the socket is created on */

#endif
