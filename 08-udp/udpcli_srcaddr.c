#include "unp.h"

/*
 * This program calls connect() on a UDP socket 
 * to be able to check the source address.
 */

int main(int argc, char *argv[])
{
    int     sockfd;
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr;
    const int on = 1;

    if (argc != 2 && argc != 3)
        err_quit("usage: udpcli_srcaddr <IPaddress> [broadcast_flag]");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* 
     * If we use a broadcast address, we need to set SO_BROADCAST.
     * Otherwise, Linux will return EPERM on attempts to connect
     * or send via a socket.
     */

    if (argc == 3)
        setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)); 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("connect() error");
        exit(2);
    }

    len = sizeof(cliaddr);
    getsockname(sockfd, (SA *) &cliaddr, &len);
    printf("local address %s\n", sock_ntop((SA *) &cliaddr, len));

    exit(0);
}
