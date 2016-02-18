#include "ping.h"

/* 
 * +------------------------------------- len ---------------------------------+
 * V                                                                           V
 * +-----------       hlenl  --------------+------------- icmplen -------------+
 * V                                       V                                   V
 * +-------------------+-------------------+------------------+----------------+
 * |  IPv4 header      |   IPv4 options    |   ICMPv4 header  |   ICMPv4 data  |
 * +-------------------+-------------------+------------------+----------------+
 */

void proc_v4(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
    int     hlenl, icmplen;
    double  rtt;
    struct  ip *ip;
    struct  icmp *icmp;
    struct  timeval *tvsend;

    ip = (struct ip *) ptr;         /* start of IP header */
    hlenl = ip->ip_hl << 2;         /* length of IP header in bytes (IP header length is stored in 4-byte units) */
    if (ip->ip_p != IPPROTO_ICMP)
        return;                     /* not ICMP */

    icmp = (struct icmp *) (ptr + hlenl);       /* start of ICMP header */
    if ((icmplen = len - hlenl) < 8)
        return;                                 /* malformed packet */

    /* we're mostly interested in ICMP echo reply messages */
    if (icmp->icmp_type == ICMP_ECHOREPLY) {
        if (icmp->icmp_id != pid)
            return;                     /* not a response to our ECHO_REQUEST */
        if (icmplen < 16)
            return;                     /* not enough data to use */

        tvsend = (struct timeval *) icmp->icmp_data;
        tv_sub(tvrecv, tvsend);
        rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.00;

        printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms\n",
                icmplen, sock_ntop_host(pr->sarecv, pr->salen),
                icmp->icmp_seq, ip->ip_ttl, rtt);

    /* in verbose mode, we also display some info about messages other than echo replies */
    } else if (verbose) {
        printf("%d bytes from %s: type = %d, code = %d\n",
                icmplen, sock_ntop_host(pr->sarecv, pr->salen),
                icmp->icmp_type, icmp->icmp_code);
    }
}
