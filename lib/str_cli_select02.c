#include "unp.h"

void str_cli_select(FILE *fp, int sockfd)
{
    int     maxfdp1, stdineof;
    fd_set  rset;
    char    buf[MAXLINE];
    int     n;

    stdineof = 0;
    FD_ZERO(&rset);
    for (;;) {
        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);

        FD_SET(sockfd, &rset);
        /* select() expects a COUNT of filedescriptors, not the highest descriptor number;
         * hence the + 1. */
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        if (select(maxfdp1, &rset, NULL, NULL, NULL) == -1) {
            if (errno == EINTR)
                continue;

            perror("str_cli: select() returned error");
            exit(1);
        }

        if (FD_ISSET(sockfd, &rset)) {      /* socket ready for reading */
            if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1)
                    return;         /* normal termination: we previously got EOF on stdin */
                else
                    err_quit("str_cli: server terminated prematurely");
            }

            write(fileno(stdout), buf, n);
        }

        if (FD_ISSET(fileno(fp), &rset)) {      /* have input data (interactive or file) */
            if ((n = read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);      /* send FIN segment */
                FD_CLR(fileno(fp), &rset);
                continue;
            }

            writen(sockfd, buf, n);
        }
    }
}
