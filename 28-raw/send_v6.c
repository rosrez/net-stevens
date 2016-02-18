#include "ping.h"

void send_v6()
{
    int     len;
    struct icmp6_hdr *icmp6;

    icmp6 = (struct icmp6_hdr *) sendbuf;
    icmp6->icmp6_type = ICMP6_ECHO_REQUEST;         /* send an ICMP echo request */
    icmp6->icmp6_code = 0;
    icmp6->icmp6_id = pid;
    icmp6->icmp6_seq = nsent++;
    memset((icmp6 + 1), 0xa5, datalen);                     /* fill payload area with pattern */
    gettimeofday((struct timeval *)(icmp6 + 1), NULL);      /* pack current time at the beginning of payload */

    len = 8 + datalen;                              /* 8-byte ICMPv6 header and data */

    /* That there is no need to calculate ICMPv6 checksum as opposed to ICMPv4; the kernel will do that */
    Sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen);
}
