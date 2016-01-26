#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    int     dgcount;
    struct sockaddr_in servaddr;

    if (argc < 2)
        err_quit("Usage: udpcli <IPaddress> [datagram_count]");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    /* convert string representation from command line to binary address in network order */
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    (argc == 3) ? dgcount = atoi(argv[2]) : 2000;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    dg_cli_loop(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr), dgcount);

    return 0;
}
