#include "unp.h"

void dg_cli_connect(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int     n;
    char    sendline[MAXLINE], recvline[MAXLINE];

    /* 
     * The call to connect() does not involve any network interaction;
     * the socket is merely assigned a port and an IP address.
     * Any failure to exchange data with the peer will be detected
     * by the subsequent calls to write(), which does send the datagram.
     * or read(), which will get errors like ECONNREFUSED if the
     * server is not running on a port.
     */
    if (connect(sockfd, (SA *) pservaddr, servlen) == -1) {
        perror("connect() error");
        exit(2);
    }

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if ((n = write(sockfd, sendline, strlen(sendline))) == -1) {
            perror("write() error");
            exit(2);
        }

        if ((n = read(sockfd, recvline, MAXLINE)) == -1) {
            perror("read() error");
            exit(2);
        }

        recvline[n] = 0;    /* terminating null character */
        fputs(recvline, stdout);
    }
}
