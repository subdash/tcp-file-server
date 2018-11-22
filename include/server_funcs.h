#ifndef SERVER_FUNCS_H
#define SERVER_FUNCS_H

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sizes.h"

/* Constructs an array of strings representing the files in the directory
 * /static and returns a pointer to it.
 */
char** static_file_tree(void);

void greet_client(char** files, int client_sockfd);

/* Iterates over an array of strings (file_list) comparing each to str. Returns
 * 1 if a match is found, 0 otherwise.
 */
int match(const char* str, char** file_list);

/* Opens a static file based on the string file_requested, and writes it to the
 * file descriptor fd if parameter valid is non-zero. Other writes
 * "Invalid request\n" to fd. Returns 0 on success, -1 on failure.
 */
int serve_client(const char* file_requested, int fd, int valid);

#endif
