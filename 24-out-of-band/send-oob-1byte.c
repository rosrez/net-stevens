/*
 * Stevens, 24.6.
 * Simple out-of-band sending program: sends just 1 byte of OOB along with normal data.
 */

#include "unp.h"

int main(int argc, char *argv[])
{
    int sockfd;

    if (argc != 3)
        err_quit("usage: send-oob-1byte <host> <port#>");

    sockfd = tcp_connect(argv[1], argv[2]);

    Write(sockfd, "123", 3);
    printf("wrote 3 bytes of normal data\n");
    sleep(1);

    Send(sockfd, "4", 1, MSG_OOB);
    printf("wrote 1 byte of OOB data\n");
    sleep(1);

    Write(sockfd, "5", 1);
    printf("wrote 1 byte of normal data\n");

    exit(0);
}
