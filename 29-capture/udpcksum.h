#include "unp.h"
#include <pcap.h>

#include <netinet/in_systm.h>       /* required for ip.h */
#include <netinet/ip.h>
// #include <netinet/ip_var.h>
#include <netinet/udp.h>
// #include <netinet/upd_var.h>
//#include <net/if.h>
#include <netinet/if_ether.h>

/*   
 * Overlay for ip header used by other protocols (tcp, udp).
 * Derived from BSD <netinet/ip_var.h
 */ 
struct ipovly {   
    u_char  ih_x1[9];               /* (unused) */   
    u_char  ih_pr;                  /* protocol */   
    u_short ih_len;                 /* protocol length */   
    struct  in_addr ih_src;         /* source internet address */   
    struct  in_addr ih_dst;         /* destination internet address */   
};

/*
 * UDP kernel structures and variables
 * Derived from BSD <netinet/udp_var.h
 */
struct udpiphdr {
    struct ipovly   ui_i;           /* overlaid ip structure */   
    struct udphdr   ui_u;           /* udp header */   
};   

#define ui_x1           ui_i.ih_x1   
#define ui_v            ui_i.ih_x1[0]   
#define ui_pr           ui_i.ih_pr   
#define ui_len          ui_i.ih_len   
#define ui_src          ui_i.ih_src   
#define ui_dst          ui_i.ih_dst   
#define ui_sport        ui_u.uh_sport   
#define ui_dport        ui_u.uh_dport   
#define ui_ulen         ui_u.uh_ulen   
#define ui_sum          ui_u.uh_sum

#define TTL_OUT 64              /* outgoing TTL */

/* declare global variables */
extern struct sockaddr *dest, *local;
extern socklen_t destlen, locallen;
extern int datalink;
extern char *device;
extern pcap_t *pd;
extern int rawfd;
extern int snaplen;
extern int verbose;
extern int zerosum;

/* function prototypes */
void cleanup(int);
char *next_pcap(int *);
void open_output(void);
void open_pcap(void);
void send_dns_query(void);
void udp_write(char *, int);
struct udpiphdr *udp_read(void);
void test_udp(void);
void sig_alrm(int);
