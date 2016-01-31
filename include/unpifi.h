/*
 * Stevens, 17.6
 * Our own header file for programs which need information
 * about interface configuration. We include this file 
 * instead of "unp.h" in such cases.
 */

#ifndef __unp_ifi_h
#define __unp_ifi_h

/* prevent unp.h from defining struct if_nameindex */
#define HAVE_IF_NAMEINDEX_STRUCT

#include "unp.h"
#include <net/if.h>

#define IFI_NAME 16 /* the same as IFNAMSIZE in <net/if.h> */
#define IFI_HADDR 8 /* taking into account the future EUI-64 */

struct ifi_info {
    char    ifi_name[IFI_NAME];     /* interface name, ends in newline */
    short   ifi_index;              /* interface index */
    short   ifi_mtu;                /* MTU for the interface */
    u_char  ifi_haddr[IFI_HADDR];   /* hardware address */
    u_short ifi_hlen;               /* hardware address length: 0, 6, 8 */
    short   ifi_flags;              /* constantes IFF_xxx <net/if.h> */
    short   ifi_myflags;            /* our flags IFI_xxx */
    struct  sockaddr *ifi_addr;     /* primary address */
    struct  sockaddr *ifi_brdaddr;  /* broadcast address */
    struct  sockaddr *ifi_dstaddr;  /* destination address */
    struct  ifi_info *ifi_next;     /* the next such structure in a list */
};

#define IFI_ALIAS 1     /* ifi_addr - this is an alias */

/* function prototypes */
struct ifi_info *get_ifi_info(int, int);
struct ifi_info *Get_ifi_info(int, int);
void free_ifi_info(struct ifi_info *);

#endif  /* __unp_ifi_h */
