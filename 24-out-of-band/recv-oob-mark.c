/*
 * Stevens, 24.7.
 * A receiving program which calls sockatmark(). 
 */

#include "unp.h"


void sig_urg(int);

int main(int argc, char *argv[])
{
    int listenfd, connfd, n, on = 1;
    char    buff[100];

    if (argc == 2)
        listenfd = tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("Usage: recv-oob [ <host> ] <port#>");

    /* enable the 'inline' mode: OOB is just another byte in the socket buffer */
    Setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on));

    connfd = Accept(listenfd, NULL, NULL);
    sleep(5);

    for ( ; ; ) {
        if (sockatmark(connfd))
            printf("at OOB mark\n");

        /* 
         * The read() call will read data up to the OOB mark 
         * (excluding the OOB data). The kernel will stop at the OOB mark
         * even if there is more data in the socket buffer.
         */
        if ((n = Read(connfd, buff, sizeof(buff) - 1)) == 0) {
            printf("received EOF\n");
            exit(0);
        }
        buff[n] = 0;        /* null terminate */
        printf("read %d bytes: %s\n", n, buff);
    }
}
