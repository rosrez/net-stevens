#include "unp.h"

#define DGLEN   1400    /* length of each datagram */

void dg_cli_loop(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen, int dgcount)
{
    int     i;
    char    sendline[DGLEN];

    for (i = 0; i < dgcount; i++) {
        sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
    }
}
