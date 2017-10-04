#include <stdio.h>
#include <string.h>

#define FILE_NAME "log.txt"

FILE *file;

char *clear_data(char *msg)
{
    char *pos = strchr(msg, '\n');
    while (pos)
    {
        *pos = ' ';
        pos = strchr(pos, '\n');
    }

    strcat(msg, "\n");

    return msg;
}

void log_data(char *client_addr, int client_port, char *msg)
{
    char data_to_write[1024];
    msg = clear_data(msg);
    sprintf(data_to_write, "[%s:%d] ", client_addr, client_port);
    strncat(data_to_write, msg, 512);
    file = fopen(FILE_NAME, "a+");
    fputs(data_to_write, file);
    printf("%s", data_to_write);
    fclose(file);
    memset(data_to_write, '\0', 1024);
}
