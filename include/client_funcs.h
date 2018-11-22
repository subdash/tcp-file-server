#ifndef CLIENT_FUNCS_H
#define CLIENT_FUNCS_H

#include <stdio.h>
#include <string.h>

#include "sizes.h"

void strip_newline(char* str); /* Remove newline from end of string */
void cli_request(char* filename); /* Assign user input to filename */

#endif
