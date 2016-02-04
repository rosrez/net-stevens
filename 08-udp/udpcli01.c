#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    struct sockaddr_in servaddr;
    const int on = 1;

    if (argc != 2 && argc != 3)
        err_quit("Usage: udpcli <IPaddress> [broadcast_flag]");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    /* convert string representation from command line to binary address in network order */
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    /* 
     * If we use a broadcast address, we need to set SO_BROADCAST.
     * Otherwise, Linux will return EPERM on attempts to connect
     * or send via a socket.
     */

    if (argc == 3)
        setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)); 

    dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

    return 0;
}
