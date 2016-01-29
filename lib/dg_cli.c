#include "unp.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n;
    char    sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) == -1) {
            perror("sendto() erorr");
            exit(2);
        }

        /* 
         * Will block in recvfrom() if there is an 
         * ICMP error message is sent back to the client.
         * The only way to detect *some* errors is to call connect() on a socket
         * prior to writing/reading.
         */
        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) == -1) {
            perror("recvfrom() error");
            exit(2);
        }

        recvline[n] = 0;    /* terminating null character */
        fputs(recvline, stdout);
    }
}
