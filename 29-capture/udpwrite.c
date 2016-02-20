#include "udpcksum.h"

int rawfd;                  /* raw socket to write on */

void open_output(void)
{
    int     on = 1;
    /* 
     * Need a raw socket to write our own IP datagrams to.
     * Process must have superuser privileges to create this socket.
     * Also must set IP_HDRINCL so we can write our own IP header.
     */

    rawfd = Socket(dest->sa_family, SOCK_RAW, IPPROTO_RAW);

    Setsockopt(rawfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));
}

void udp_write(char *buf, int userlen)
{
    struct udpiphdr *ui;
    struct ip *ip;

    /* fill in and checksum UDP header */
    ip = (struct ip *) buf;
    ui = (struct udpiphdr *) buf;
    bzero(ui, sizeof(*ui));
    /* add 8 to userlen for pseudoheader length */
    ui->ui_len = htons((uint16_t) (sizeof(struct udphdr) + userlen));
    /* then add 28 for IP datagram length */
    userlen += sizeof(struct udpiphdr);

    ui->ui_pr = IPPROTO_UDP;
    ui->ui_src.s_addr = ((struct sockaddr_in *) local)->sin_addr.s_addr;
    ui->ui_dst.s_addr = ((struct sockaddr_in *) dest)->sin_addr.s_addr;
    ui->ui_sport = ((struct sockaddr_in *) local)->sin_port;
    ui->ui_dport = ((struct sockaddr_in *) dest)->sin_port;
    /* 
     * copy the current IP length to UDP length field, since
     * now it includes only the UDP header payload size;
     * we'll bump the IP header length up later
     */
    ui->ui_ulen = ui->ui_len;

    if (zerosum == 0) {
        if ((ui->ui_sum = in_cksum((uint16_t *) ui, userlen)) == 0)
            ui->ui_sum = 0xffff;
    }

    /* fill in the rest of IP header */
    /* ip_output() calculates & stores IP header checksum */
    ip->ip_v = IPVERSION;
    ip->ip_hl = sizeof(struct ip) >> 2;
    ip->ip_tos = 0;
#if defined(linux) || defined(__OpenBSD__)
    ip->ip_len = htons(userlen);        /* network byte order */
#else
    ip->ip_len = userlen;               /* host byte order */
#endif
    ip->ip_id = 0;                      /* let IP set this */
    ip->ip_off = 0;                     /* frag offset, MF and DF flags */
    ip->ip_ttl = TTL_OUT;

    Sendto(rawfd, buf, userlen, 0, dest, destlen);
}
