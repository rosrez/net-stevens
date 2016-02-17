/*
 * Stevens, 24.4.
 * Receiving program that detects OOB data with select() and exception descriptor set.
 */

#include "unp.h"

int main(int argc, char *argv[])
{
    int listenfd, connfd, n, justreadoob = 0;
    char    buff[100];
    fd_set  rset, xset;

    if (argc == 2)
        listenfd = tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("Usage: recv-oob [ <host> ] <port#>");

    connfd = Accept(listenfd, NULL, NULL);

    /* read descriptor set */
    FD_ZERO(&rset);
    /* exception descriptor set */
    FD_ZERO(&xset);
    for ( ; ; ) {
        /* always check for new regular data */
        FD_SET(connfd, &rset);
        /* check for OOB data only if the previous iteration didn't read any OOB data */
        if (justreadoob == 0)
            FD_SET(connfd, &xset);

        /* 
         * The reason we need to check if we've just read OOB data is that
         * select() will be signaling exceptions until data past OOB is read.
         * We can't read OOB data twice since this would result in EINVAL.
         */

        /* OOB data will be signaled by a non-empty xset */
        Select(connfd + 1, &rset, NULL, &xset, NULL);

        /* exception on connfd: received OOB -- read it */
        if (FD_ISSET(connfd, &xset)) {
            n = Recv(connfd, buff, sizeof(buff) - 1, MSG_OOB);
            buff[n] = 0;        /* null terminate */
            printf("read %d OOB byte: %s\n", n, buff);
            justreadoob = 1;
            FD_CLR(connfd, &xset);
        }
        /* regular data arrival on connfd: just read the data */
        if (FD_ISSET(connfd, &rset)) {
            if ((n = Read(connfd, buff, sizeof(buff) - 1)) == 0) {
                printf("Received EOF\n");
                exit(0);
            }
            buff[n] = 0;        /* null terminate */
            printf("read %d bytes: %s\n", n, buff);
            justreadoob = 0;
        }
    }
}
