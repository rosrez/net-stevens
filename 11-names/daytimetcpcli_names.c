#include "unp.h"

int main(int argc, char *argv[])
{
    int     sockfd, n;
    char    recvline[MAXLINE + 2];
    socklen_t len;
    struct sockaddr_storage *ss;

    if (argc != 3)
        err_quit("Usage: daytimetcpcli_names <hostname/IPaddress> <service/port#>");

    sockfd = tcp_connect(argv[1], argv[2]);

    len = sizeof(ss);
    getpeername(sockfd, (SA *) &ss, &len);
    printf("connected to %s\n", sock_ntop_host((SA *) &ss, len));

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n++] = '\n';
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
    exit(0);
}
