#include "unp.h"

int main(int argc, char *argv[])
{
    int     i, maxi, maxfd, listenfd, connfd, sockfd;
    int     nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set  rset, allset;
    char    buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    maxfd = listenfd;       /* initialization */
    maxi = -1;              /* index into the client[] array */
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;      /* -1 denotes a free element */

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for (;;) {
        rset = allset;          /* assignment to structure */
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {        /* new connection */
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (SA *) &cliaddr, &clilen);

            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0) {
                    client[i] = connfd;         /* save the descriptor */
                    break;
                }

            if (i == FD_SETSIZE)
                err_quit("too many clients");

            FD_SET(connfd, &allset);            /* adding a new descriptor */
            if (connfd > maxfd)
                maxfd = connfd;                 /* for select() */
            if (i > maxi)
                maxi = i;                       /* maximum index into client[] */

            if (--nready <= 0)
                continue;                       /* no more descriptors ready for reading */
        }

        for (i = 0; i <= maxi; i++) {           /* check all clients for data */
            if ((sockfd = client[i]) < 0)
                continue;

            if (FD_ISSET(sockfd, &rset)) {
                if ((n = read(sockfd, buf, MAXLINE)) == 0) {    /* connection closed by client */
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    writen(sockfd, buf, n);
                }

                if (--nready <= 0)
                    break;          /* no more descriptors ready for reading */
            }

        }
    }
}
