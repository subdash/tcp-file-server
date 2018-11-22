#include "client_funcs.h"

void strip_newline(char* str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

void cli_request(char* filename)
{
    fgets(filename, MAX_NAME_SZ, stdin);
    if (strlen(filename) > 1)
        strip_newline(filename);
}
