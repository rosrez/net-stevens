#include "unp.h"

/* 
 * This is the same code as the TCP client from Chapter 5.
 * The only difference is that we call the nonblocking
 * version of str_cli(): str_cli_nonb().
 */

int main(int argc, char *argv[])
{
    int     sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2)
        err_quit("usage: tcpcli <IPAddress>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    str_cli_nonb(stdin, sockfd);     /* this function does all client processing */

    exit(0);
}
