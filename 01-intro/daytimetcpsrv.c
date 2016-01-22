#include "unp.h"
#include <time.h>

/*
 * Stevens, Listing 1.2
 * TCP server for date/time.
 */

int main(int argc, char *argv[])
{
    int     listenfd, connfd;
    struct  sockaddr_in servaddr, cliaddr;
    char    buff[MAXLINE];
    time_t  ticks;
    socklen_t len;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);      /* time and date server */

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    /* 
     * Stevens omits the following line in the loop:
     *
     * len = sizeof(cliaddr);
     *
     * This line is important because the address returned by accept()
     * gets truncated to the initial length specified in this system call.
     * Without this line, this program returned 0.0.0.0 port 0 as the client
     * address. This behaviour might be Linux-specific.
     */

    for (;;) {
        len = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *) &cliaddr, &len);

        printf("connection from %s, port %d\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
                ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\er\en", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }
}
