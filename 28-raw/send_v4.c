#include "ping.h"

void send_v4(void)
{
    int     len;
    struct  icmp *icmp;

    icmp = (struct icmp *) sendbuf;
    icmp->icmp_type = ICMP_ECHO;        /* send an ICMP echo request */
    icmp->icmp_code = 0;
    icmp->icmp_id = pid;                /* to tell responses to our requests from other ICMP responses */
    icmp->icmp_seq = nsent++;           /* our sequence number (just increment each time we send a request) */
    memset(icmp->icmp_data, 0xa5, datalen);                     /* fill payload area with pattern */
    gettimeofday((struct timeval *) icmp->icmp_data, NULL);     /* pack current time at the beginning of payload */

    len = 8 + datalen;                  /* checksum ICMP header (8 bytes) and data */
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

    Sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
}
