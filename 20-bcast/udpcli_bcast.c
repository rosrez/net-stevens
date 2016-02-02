#include "unp.h"
#include "dgcli.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2 && argc != 3)
        err_quit("Usage: udpcli <IPaddress> [port]");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (argc > 2)
        servaddr.sin_port = htons(atoi(argv[2]));
    /* convert string representation from command line to binary address in network order */
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    dg_cli_bcast(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

    return 0;
}
