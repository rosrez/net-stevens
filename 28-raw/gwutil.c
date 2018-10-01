#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/ioctl.h>          // ioctl()
#include <bits/ioctls.h>        // defines values for argument "request" of ioctl
#include <net/if.h>             // struct ifreq
#include <linux/if_ether.h>     // ETH_P_ARP = 0x0806
#include <linux/if_packet.h>    // struct sockaddr_ll (see man 7 packet)
#include <net/ethernet.h>
#include <netinet/ip.h>         // IP_MAXPACKET

#include "gwutil.h"

#define DEBUG printf

/* Define some constants */
#define ETH_HDRLEN 14       /* Ethernet header length */
#define IP4_HDRLEN 20       /* IPv4 header length */
#define ARP_HDRLEN 28       /* ARP header length */

#define ARPOP_REQUEST 1     /* Taken from <linux/if_arp.h> */
#define ARPOP_REPLY 2           /* Taken from <linux/if_arp.h> */

/* Define a struct for ARP header */
typedef struct arp_hdr {
    uint16_t    htype;
    uint16_t    ptype;
    uint8_t     hlen;
    uint8_t     plen;
    uint16_t    opcode;
    uint8_t     sender_mac[6];
    uint8_t     sender_ip[4];
    uint8_t     target_mac[6];
    uint8_t     target_ip[4];
} arp_hdr;


int gateway_enum(gateway_info *gw, int numitems)
{
    char interface[GW_INTF_LEN];
    int cnt;
    uint32_t dest, ip;
    FILE *f;
    char l[132];

    memset(gw, 0, sizeof(gateway_info) * numitems);

    f = fopen("/proc/net/route", "r");
    if (!f)
        return -1;

    cnt = 0;
    while (cnt < numitems && fgets(l, sizeof(l), f) != NULL) {
        memset(interface, 0, sizeof(interface));

        /* Skip the "Iface Destination etc. line */
        if (strncmp(l, "Iface", 5) == 0)
            continue;

        /* Skip anything that doesn't conform to our format */
        if (sscanf(l, "%s %08X %08x", interface, &dest, &ip) != 3)
            continue;

        /* If destination is not 0.0.0.0, this is not a gateway entry */
        if (dest != 0)
            continue;

        strncpy(gw->interface, interface, sizeof(interface));
        gw->ip = ip;
        gw++;
        cnt++;
    }

    fclose(f);
    return cnt;
}

char *mac2str(unsigned char *mac)
{
    static char buf[18];

    snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
        *mac, *(mac+1), *(mac+2), *(mac+3), *(mac+4), *(mac+5));
    return buf;
}

/* Assumes network byte order */

char *ip2str(uint32_t *ip)
{
    static char buf[16];
    unsigned char *ipptr = (unsigned char *) ip;
    snprintf(buf, sizeof(buf), "%u.%u.%u.%u", *ipptr, *(ipptr+1), *(ipptr+2), *(ipptr+3));
    return buf;
}

void gateway_fprint(FILE *f, gateway_info *gw)
{
    fprintf(f, "%s %s %s\n", gw->interface, ip2str(&gw->ip), mac2str(&gw->mac[0]));
}

int gateway_get_mac(gateway_info *gw, int timeout)
{
    int     sd, nsent, rc, frame_length; 
    unsigned char src_mac[6], dst_mac[6];
    unsigned char src_ip[4];
    arp_hdr send_arphdr;
    arp_hdr *recv_arphdr;
    unsigned char *ether_frame;
    struct ifreq ifr;
    struct sockaddr_ll device;

    ether_frame = malloc(65536);    /* TODO: adjust size */

    /* Submit request for a socket descriptor to look up interfaces */
    if ((sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        DEBUG("socket() failed to get socket for ioctl() ");
        goto error;
    }

    /* Use ioctl() to look up interface name and get its MAC address */
    printf("Determining MAC address for %s\n", gw->interface);
    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", gw->interface);
    if (ioctl(sd, SIOCGIFHWADDR, &ifr) < 0) {
        DEBUG("ioctl() failed to get source MAC address ");
        goto error;
    }
    /* Copy source MAC address */
    memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);

    /* Get interface IP address */
    if (ioctl(sd, SIOCGIFADDR, &ifr) < 0) {
        DEBUG("ioctl() failed to get source IP address ");
        goto error;
    }
    memcpy(src_ip, &((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr, 4);
    close(sd);

    /* Report source MAC/IP addresses to stdout */
    printf("MAC address for interface %s is %s\n", gw->interface, mac2str(&src_mac[0]));
    printf("IP address for interface %s is %s\n", gw->interface, ip2str((uint32_t *) &src_ip));

    /* Find interface index from interface name and store index in struct sockaddr_ll device,
     * which will be used as an argument of sendto() */
    memset(&device, 0, sizeof(device));
    if ((device.sll_ifindex = if_nametoindex(gw->interface)) == 0) {
        DEBUG("if_nametoindex() failed to obtain interface index ");
        goto error;
    }
    printf("Index for interface %s is %i\n", gw->interface, device.sll_ifindex);

    /* Set destination MAC address: broadcast address */
    memset(dst_mac, 0xff, 6);

    /* Source IP address */
    memcpy(&send_arphdr.sender_ip, &src_ip, 4);
    /* Target IP address */
    memcpy(&send_arphdr.target_ip, &gw->ip, 4);   /* TODO! */

    /* Fill out sockaddr_ll */
    device.sll_family = AF_PACKET;
    memcpy(device.sll_addr, src_mac, 6);
    device.sll_halen = 6;

    /* ARP header */
    
    /* Hardware type (16 bits): 1 for ethernet */
    send_arphdr.htype = htons(1);
    /* Protocol type (16 bits): 2048 for IP */
    send_arphdr.ptype = htons(ETH_P_IP);
    /* Hardware address length (8 bits): 6 bytes for MAC address */
    send_arphdr.hlen = 6;
    /* Hardware address length (8 bits): 4 bytes for IPv4 address */
    send_arphdr.plen = 4;
    /* OpCode: 1 for ARP request */
    send_arphdr.opcode = htons(ARPOP_REQUEST);
    /* Sender hadrware address (48 bits): MAC address */
    memcpy(&send_arphdr.sender_mac, src_mac, 6);

    /* Sender protocol address (32 bits)
     * See getaddrinfo() resolution of src_ip */

    /* Target hardware address (48 bits): 
     * zero, since we don't know it yet. */
    memset(&send_arphdr.target_mac, 0, 6);

    /* Target protocol adddress (32 bits)
     * See getaddrinfo() resolution of target. */

    /* --- Fill out ethernet frame header --- */

    /* Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (ARP header) */
    frame_length = 6 + 6 + 2 + ARP_HDRLEN;

    /* Destination and source MAC addresses */
    memcpy(ether_frame, dst_mac, 6);
    memcpy(ether_frame + 6, src_mac, 6);

    /* Next comes the ethernet type code (ETH_P_ARP for ARP).
     * See http://www.iana.org/assignments/ethernet-numbers */
    ether_frame[12] = ETH_P_ARP / 256;
    ether_frame[13] = ETH_P_ARP % 256;

    /* Next is ethernet frame data (ARP header) */
    memcpy(ether_frame + ETH_HDRLEN, &send_arphdr, ARP_HDRLEN);

    /* Submit a request for the raw socket descriptor */
    if ((sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        DEBUG("socket() failed");
        goto error;
    }

    /* Send ethernet frame to socket */
    if ((nsent = sendto(sd, ether_frame, frame_length, 0, (struct sockaddr *) &device, sizeof(device))) <= 0) {
        DEBUG("sendto() failed ");
        goto error;
    }

    /* Set socket timeout for read operations to user-supplied value, if present */
    if (timeout) {
        struct timeval tv;
        tv.tv_sec = timeout / 1000000;
        tv.tv_usec = timeout % 1000000;
        setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }

    memset(ether_frame, 0, frame_length);

    /* 
     * Listen for incoming ethernet frame from socket sd.
     * We expect an ARP ethernet frame of the form:
     *      MAC (6 bytes) + MAC (6 bytes) + ethernet type (2 bytes)
     *      + ethernet data (ARP header) (28 bytes).
     *  Loop until we get an ARP reply
     */
    recv_arphdr = (arp_hdr *) (ether_frame + 6 + 6 + 2);
    while (((((ether_frame[12] << 8) + ether_frame[13]) != ETH_P_ARP) || htons(recv_arphdr->opcode) != ARPOP_REPLY)) {
        if ((rc = recv(sd, ether_frame, IP_MAXPACKET, 0)) < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                /* recv() timed out */
                printf("Operation timed out\n");
                exit(EXIT_SUCCESS);
            } else if (errno == EINTR) {
                /* Interrupted by signal */
                memset(ether_frame, 0, IP_MAXPACKET);
                continue;
            } else {
                /* Other failure */
                perror("recv() failed ");
                exit(EXIT_FAILURE);
            }
        }

        if (memcmp(recv_arphdr->sender_ip, send_arphdr.target_ip, 4) == 0 &&
                memcmp(recv_arphdr->target_ip, send_arphdr.sender_ip, 4) == 0) {
            /* We're sure we have a response to OUR request */
            memcpy(gw->mac, recv_arphdr->sender_mac, 6);
        }
    }
    close(sd);


    /* Close socket descriptor */
    close(sd);

    /* Free allocated memory */
    free(ether_frame);
    return 0;   

error:
    close(sd);
    free(ether_frame);
    return -1;
}
