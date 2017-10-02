#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "recv.c"

#define PORT 2121
#define BANNER "220 (vsFTPd 2.3.4)\n"
#define LOGIN "530 Please login with USER and PASS.\n"
#define USER "331 User name OK, Enter PASS command\n"
#define PASS "550 Permission denied.\n"

int main(void)
{
    int sockfd, new_sockfd;
    struct sockaddr_in host_addr, client_addr;
    socklen_t sin_size;
    int recv_length=1, yes=1;
    char buffer[1024];

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket fail");
        exit(-1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("socket fail");
        exit(-1);
    }

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(host_addr.sin_zero), '\0', 8);

    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("socket create fail");
        exit(-1);
    }

    if (listen(sockfd, 5) == -1)
    {
        perror("socket listen fail");
        exit(-1);
    }

    printf("Server runnig on port %i\n", PORT);

    while(1)
    {
        sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

        if(new_sockfd == -1)
        {
            perror("socket accepting fail");
            exit(-1);
        }

        //printf("[%s:%d]\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)); //Add to log
        log_data(inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), "Connected\n");
        send(new_sockfd, BANNER, sizeof(BANNER), 0);
        memset(&buffer, '\0', 1024);
        recv_length = recv(new_sockfd, &buffer, 1024, 0);

        while(recv_length > 0) {
            if(strcasestr(&buffer, "USER "))
            {
                send(new_sockfd, USER, sizeof(USER), 0);
            }
            else if(strcasestr(&buffer, "PASS "))
            {
                send(new_sockfd, PASS, sizeof(PASS), 0);
            }
            else
            {
                send(new_sockfd, LOGIN, sizeof(LOGIN), 0);
            }
            log_data(inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
            memset(&buffer, '\0', 1024);
            recv_length = recv(new_sockfd, &buffer, 1024, 0);
        }

        close(new_sockfd);  //close is ok?
    }

    return 0;
}
