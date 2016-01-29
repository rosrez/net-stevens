#include "unp.h"

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
    int     n;
    socklen_t len;
    char    mesg[MAXLINE];

    for (;;) {
        len = clilen;
        if ((n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len)) == -1) {
            perror("recvfrom() error");
            exit(2);
        }

        if (sendto(sockfd, mesg, n, 0, pcliaddr, len) == -1) {
            perror("sendto() error");
            exit(2);
        }
    }
}
