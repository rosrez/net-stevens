/*
 * Stevens, 24.9.
 * Receiving program that detects OOB data with the SIGUSR signal
 * and reads OOB data in the signal handler.
 * The program is supposed to be used with sender of bigger data (see 24.8).
 */

#include "unp.h"

int listenfd, connfd;

void sig_urg(int);

int main(int argc, char *argv[])
{
    int     size;

    if (argc == 2)
        listenfd = tcp_listen(NULL, argv[1], NULL);
    else if (argc == 3)
        listenfd = tcp_listen(argv[1], argv[2], NULL);
    else
        err_quit("Usage: recv-oob-big [ <host> ] <port#>");

    /* 
     * Since our sender will send a 16K chunk followed by OOB byte
     * and we set the socket receive buffer to just 4K, we are 
     * guaranteed to fill the receiving socket buffer first 
     * and receive SIGURG while the actual urgent data has not yet been received.
     */
    size = 4096;
    Setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    connfd = Accept(listenfd, NULL, NULL);

    /* the process will be receiving SIGURG on delivery of urgent segments */
    signal(SIGURG, sig_urg);
    /* the process will be notified by SIGURG only if it declares itself owner of the socket */
    fcntl(connfd, F_SETOWN, getpid());

    for ( ; ; ) {
        /* wait for signals */
        pause();
    }
}

/* 
 * We'll receive a segment with the urgent flag set and therefore SIGURG 
 * before the kernel receives the actual OOB byte. The attempt to read the byte
 * will result in EWOULDBLOCK/EAGAIN (Resource temporarily unavailable).
 */
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
