#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>             /* close() */
#include <string.h>             /* strcpy(), memset() */

#include <netinet/ip.h>         /* IP_MAXPACKET (65535) */
#include <sys/types.h>          /* needed for socket(), uint8_t, uint16_t */
#include <sys/socket.h>         /* socket() */
#include <net/ethernet.h>       /* ETH_P_ARP = 0x0806, ETH_P_ALL = 0x0003 */

#include <errno.h>              /* errno, perror() */

/* Define a struct for ARP header */
typedef struct _arp_hdr {
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

#define ARPOP_REPLY 2           /* Taken from <linux/if_arp.h> */

char *mac2str(unsigned char *mac)
{
    static char buf[18];

    snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
        *mac, *(mac+1), *(mac+2), *(mac+3), *(mac+4), *(mac+5));
    return buf;
}

char *ip2str(unsigned char *ip)
{
    static char buf[16];
    snprintf(buf, sizeof(buf), "%u.%u.%u.%u", *ip, *(ip+1), *(ip+2), *(ip+3));
    return buf;
}

int main(int argc, char **argv)
{
    int     sd, status;
    uint8_t *ether_frame;
    arp_hdr *arphdr;

    ether_frame = malloc(IP_MAXPACKET);
    if (!ether_frame) {
        perror("malloc() failed ");
        exit(EXIT_FAILURE);
    }

    /* Request a raw socket descriptor */
    if ((sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        perror("socket() failed ");
        exit(EXIT_FAILURE);
    }

    /* Set socket timeout for read operations to user-supplied value, if present */
    if (argv[1]) {
        struct timeval tv;
        tv.tv_sec = atoi(argv[1]);
        tv.tv_usec = 0;
        setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }

    /* 
     * Listen for incoming ethernet frame from socket sd.
     * We expect an ARP ethernet frame of the form:
     *      MAC (6 bytes) + MAC (6 bytes) + ethernet type (2 bytes)
     *      + ethernet data (ARP header) (28 bytes).
     *  Loop until we get an ARP reply
     */
    arphdr = (arp_hdr *) (ether_frame + 6 + 6 + 2);
    while (((((ether_frame[12] << 8) + ether_frame[13]) != ETH_P_ARP) || htons(arphdr->opcode) != ARPOP_REPLY)) {
        if ((status = recv(sd, ether_frame, IP_MAXPACKET, 0)) < 0) {
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
    }
    close(sd);

    /* Print out contents of received ethernet frame */
    printf("\nEthernet frame header:\n");
    printf("Destination MAC (this node): %s\n", mac2str(&ether_frame[0]));
    printf("Source MAC: %s\n", mac2str(&ether_frame[6]));

    printf("Ethernet type code (2054 = ARP): %u\n", ((ether_frame[12] << 8) + ether_frame[13]));
    
    printf("\nEthernet data (ARP header):\n");
    printf("Hardware type (1 = ethernet (10 Mb)): %u\n", ntohs(arphdr->htype));
    printf("Protocol type (2048 for IPv4 addresses): %u\n", ntohs(arphdr->ptype));
    printf("Hardware (MAC) address length (bytes): %u\n", arphdr->hlen);
    printf("Protocol (IPv4) address length (bytes): %u\n", arphdr->plen);
    printf("Opcode (2 = ARP reply): %u\n", ntohs(arphdr->opcode));
    
    printf("Sender hardware (MAC) address: %s\n", 
            mac2str(&arphdr->sender_mac[0]));
    printf("Sender protocol (IPv4) address: %s\n",
            ip2str(&arphdr->sender_ip[0]));
    printf("Destination (this node) hardware (MAC) address: %s\n", 
            mac2str(&arphdr->target_mac[0]));
    printf("Destination (this node) protocol (IPv4) address: %s\n",
            ip2str(&arphdr->target_ip[0]));

    free(ether_frame);

    exit(EXIT_SUCCESS);
}
