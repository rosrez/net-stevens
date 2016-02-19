#include "udpcksum.h"

struct udpiphdr *udp_read(void)
{
    int     len;
    char    *ptr;
    struct ether_header *eptr;

    for ( ; ; ) {
        ptr = next_pcap(&len);

        switch (datalink) {
        /* loopback header = 4 bytes */
        case DLT_NULL:
            return upd_check(ptr + 4, len - 4);

        /* Ethernet */
        case DLT_EN10MB:
            eptr = (struct ether_header *) ptr;
            if (ntohs(eptr->ether_type) != ETHERTYPE_IP)
                err_quit("Ethernet type %x not IP", htons(eptr->ether_type));
            return udp_check(ptr + 14, len - 14);

        /* SLIP header = 24 bytes */
        case DLT_SLIP:
            return udp_check(ptr + 24, len - 24);

        /* PPP header = 24 bytes */
        case DLT_PPP:
            return udp_check(ptr + 24, len - 24);

        default:
            err_quit("unsupported datalink (%d)", datalink);
        }
    }
}

struct udpiphdr *udp_check(char *ptr, int len)
{
    int     hlen;
    struct ip *ip;
    struct udpiphdr *ui;

    if (len < sizeof(struct ip) + sizeof(struct udphdr))
        err_quit("len = %d", len);

    /* minimal verification of IP header */
    ip = (struct ip *) ptr;
    if (ip->ip_v != IPVERSION)
        err_quit("ip_v = %d", ip->ip_v);

    hlen = ip->ip_hl << 2;
    if (hlen < sizeof(struct ip))
        err_quit("ip_hl = %d", ip_hl);

    if (len < hlen + sizeof(struct udphdr))
        err_quit("len = %d, hlen = %d", len, hlen);

    if ((ip->ip_sum = in_cksum((uint16_t *) ip, hlen)) != 0)
        err_quit("ip checksum error");

    if (ip->ip_p == IPPROTO_UDP) {
        ui = (struct udpiphdr *) ip;
        return ui;
    } else {
        err_quit("not a UDP packet");
    }
}
