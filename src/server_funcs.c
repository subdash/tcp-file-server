#define _POSIX_C_SOURCE 200112L
#include "server_funcs.h"
#include <sys/stat.h>

char** static_file_tree(void)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char** files;
    int num_files, i;

    /* Initialize list of strings to store file names */
    if ((files = malloc(128 * sizeof(char*))) == NULL)
    {
        perror(__FILE__ " static_file_tree()");
        return (char **)0;
    }
    /* Sanitize the allocated memory */
    memset(files, '\0', 128 * sizeof(char*));
    for (i = 0; i < 128; ++i)
    {
        files[i] = malloc((MAX_NAME_SZ + 1) * sizeof(char));
        memset(files[i], '\0', sizeof(char));
    }
    /* Open the static directory */
    if ((dp = opendir("static")) == NULL)
    {
        fprintf(stderr, "cannot open directory: static\n");
        return (char **)0;
    }
    
    /* cd into /static directory and build list of files */
    chdir("static");
    num_files = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        /* Ignore current and parent directories */
        if (strcmp(".", entry->d_name) == 0 ||
            strcmp("..", entry->d_name) == 0)
            continue;
        
        lstat(entry->d_name, &statbuf);
        strcpy(files[num_files], entry->d_name);
        ++num_files;
    }
    /* cd back into main directory and close the direcetory */
    chdir("..");
    closedir(dp);
    
    return files;
}

int match(const char* str, char** file_list)
{
    int i = 0;
    while (file_list[i][0] != '\0')
    {
        if (strncmp(file_list[i], str, MAX_NAME_SZ) == 0)
            return 1;
        ++i;
    }
    return 0;
}

int serve_client(const char* file_requested, int fd, int valid)
{
    char file_contents[MAX_MSG_SZ];
    memset(file_contents, '\0', sizeof(file_contents));

    /* If request was valid, serve corresponding file */
    if (valid)
    {
        FILE* fp;
        char filepath[MAX_NAME_SZ + 7] = "static/";
        printf("[SERVER] serving client %s on fd %d\n", file_requested, fd);
        strncat(filepath, file_requested, (MAX_NAME_SZ + 7));
        if ((fp = fopen(filepath, "r")) == NULL)
        {
            fprintf(stderr, "[SERVER] error opening %s", file_requested);
            return -1;
        }
        fread(file_contents, MSG_CHUNK_SZ, MAX_STATIC_FILES, fp);
        write(fd, &file_contents, sizeof(file_contents));
        fclose(fp);
    }
    /* If request was not valid, skip file I/O and alert client */
    else
    {
        printf("[SERVER] client %d requested invalid file '%s'\n", fd,
            file_requested);
        strcpy(file_contents, "Invalid request\n");
        write(fd, &file_contents, sizeof(file_contents));
    }
    return 0;
}

void greet_client(char** files, int client_sockfd)
{
    int i;
    for (i = 0; i < MAX_STATIC_FILES; ++i) {
        if (files[i])
            write(client_sockfd, files[i], MAX_MSG_SZ);
        else
            break;
    }
    printf("[SERVER] done greeting client %d\n", client_sockfd);
}
