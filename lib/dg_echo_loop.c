#include "unp.h"

static void recvfrom_int(int);
static int count;

void dg_echo_loop(int sockfd, SA *pcliaddr, socklen_t clilen, int sleep_time)
{
    socklen_t len;
    char    mesg[MAXLINE];

    signal(SIGINT, recvfrom_int);

    for (;;) {
        len = clilen;
        recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

        if (sleep_time)
            usleep(sleep_time);

        count++;
    }
}

static void recvfrom_int(int signo)
{
    printf("\enreceived %d datagrams\n", count);
    exit(0);
}
