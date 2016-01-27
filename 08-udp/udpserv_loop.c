#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd;
    int     sleep_time;
    struct  sockaddr_in servaddr, cliaddr;

    argc == 2 ? sleep_time = atoi(argv[1]) : 0;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    /* never returns */
    dg_echo_loop(sockfd, (SA *) &cliaddr, sizeof(cliaddr), sleep_time);

    /* just to keep the compiler happy */
    return 0;
}
