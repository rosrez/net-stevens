/*
 * Stevens, 24.2.
 * Receiving program that detects OOB data with the SIGUSR signal
 * and reads OOB data in the signal handler.
 */

#include "unp.h"

int listenfd, connfd;

void sig_urg(int);

int main(int argc, char *argv[])
{
    int     n;
    char    buff[100];

    if (argc == 2)
        listenfd = tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("Usage: recv-oob [ <host> ] <port#>");

    connfd = Accept(listenfd, NULL, NULL);

    /* the process will be receiving SIGURG on delivery of urgent segments */
    signal(SIGURG, sig_urg);
    /* the process will be notified by SIGURG only if it declares itself owner of the socket */
    fcntl(connfd, F_SETOWN, getpid());

    for ( ; ; ) {
        if ((n = Read(connfd, buff, sizeof(buff) - 1)) == 0) {
            printf("received EOF\n");
            exit(0);
        }
        buff[n] = 0;        /* null terminate */
        printf("read %d bytes: %s\n", n, buff);
    }
}

void sig_urg(int signo)
{
    int     n;
    char    buff[100];

    printf("SIGURG received\n");
    /* read out-of-bound data */
    n = Recv(connfd, buff, sizeof(buff) - 1, MSG_OOB);
    buff[n] = 0;            /* null terminate */
    printf("read %d OOB byte: %s\n", n, buff);
}
