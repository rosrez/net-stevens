#include "unpifi.h"

#include <sys/ioctl.h>

/* Stevens 17.4-17.8 
 * This function enumerates network interfaces and collects their characteristics
 * such as names, MTU, broadcast addresses, and so on. 
 * There is a function in Linux that does just that: getifaddrs(3). It also 
 * returns a linked list of interface-related data. It is also paired with 
 * the freeifaddrs() function to free memory occupied by the list. */


/* 17.4 */

struct ifi_info *get_ifi_info(int family, int doaliases)
{
    struct  ifi_info *ifi, *ifihead, **ifipnext;
    int     sockfd, len, lastlen, flags, myflags, idx = 0, hlen = 0;
    char    *ptr, *buf, lastname[IFNAMSIZ], *cptr, *haddr, *sdlname;
    struct  ifconf  ifc;
    struct  ifreq   *ifr, ifrcopy;
    struct  sockaddr_in     *sinptr;
    struct  sockaddr_in6    *sin6ptr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    lastlen = 0;
    /* initial approximation of buffer size */
    len = 100 * sizeof(struct ifreq);
    for (;;) {
        buf = malloc(len);
        ifc.ifc_len = len;
        ifc.ifc_buf = buf;
        if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
            if (errno != EINVAL || lastlen != 0)
                err_sys("ioctl() error");
        } else {
            if (ifc.ifc_len == lastlen)
                break;      /* success, len value hasn't changed */

            lastlen = ifc.ifc_len;
        }

        /* exponential growth */
        len += 10 * sizeof(struct ifreq);
        free(buf);
    }

    ifihead = NULL;
    ifipnext = &ifihead;
    lastname[0] = 0;
    sdlname = NULL;

    /* 17.5 ----- */

    for (ptr = buf; ptr < buf + ifc.ifc_len; ) {
        ifr = (struct ifreq *) ptr;

/* 
 * This complex logic provided by Stevens doesn't work for
 * modern Linux. Linux just populates the buffer with 
 * 'struct ifreq's.
 */

#if 0
#//ifdef HAVE_SOCKADDR_SA_LEN
        len = max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
#//else   /* Linux */
        switch(ifr->ifr_addr.sa_family) {
            case AF_INET6:
                len = sizeof(struct sockaddr_in6);
                break;

            case AF_INET:
                /* fall through */

            default:
                len = sizeof(struct sockaddr);
                break;
        }
//#endif
#endif
        /* Linux: just skip over to next struct */
        ptr += sizeof(*ifr);     /* for next entry in buffer */

        /* Note AF_PACKET on Linux corresponds to AF_LINK on FreeBSD,
         * used by Stevens in his code. sockaddr_ll is the Linux's
         * counterpart of sockaddr_ll used by FreeBSD.
         * Replaced all FreeBSD types/constants with Linux-specific. */

#ifdef AF_PACKET 
        /* we assume, that AF_PACKET comes before AF_INET and AF_INET6 */
        if (ifr->ifr_addr.sa_family == AF_PACKET) {
            struct sockaddr_ll *sll = (struct sockaddr_ll *) &ifr->ifr_addr;
            sdlname = ifr->ifr_name;
            idx = sll->sll_ifindex;                 /* BSD: sdl->sdl_index; */
            haddr = (char *) &sll->sll_addr[0];     /* BSD: sdl->sdl_data + sdl->sdl_nlen; */
            hlen =  sll->sll_halen;                 /* BSD: sdl->sdl_alen; */
        }
#endif

        if (ifr->ifr_addr.sa_family != family)
            continue;       /* ignore if not desired address family */

        myflags = 0;
        if ((cptr = strchr(ifr->ifr_name, ':')) != NULL)
            *cptr = 0;      /* replace colon with null */
        if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0) {
            if (!doaliases)
                continue;   /* already processed this interface */
            myflags = IFI_ALIAS;
        }
        memcpy(lastname, ifr->ifr_name, IFNAMSIZ);

        ifrcopy = *ifr;
        ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
        flags = ifrcopy.ifr_flags;
        if ((flags & IFF_UP) == 0)
            continue;       /* ignore if interface is not up */

        /* 17.6 --------- */

        ifi = calloc(1, sizeof(struct ifi_info));
        *ifipnext = ifi;             /* prev points to the new structure */
        ifipnext = &ifi->ifi_next;  /* the pointer to next structure points here */

        ifi->ifi_flags = flags;         /* IFF_xxx values */
        ifi->ifi_myflags = myflags;     /* IFI_xxx values */
#if defined(SIOCGIFMTU) && defined(HAVE_STRUCT_IFREQ_IFR_MTU)
        if (ioctl(sockfd, SIOCGIFMTU, &ifrcopy) == 0)
            ifi->ifi_mtu = ifrcopy.ifr_mtu;
        else
            perror("ioctl() for SIOCIFMTU failed");
#else
        ifi->ifi_mtu = 0;
#endif
        memcpy(ifi->ifi_name, ifr->ifr_name, IFI_NAME);
        ifi->ifi_name[IFI_NAME - 1] = '\0';
        /* if sockaddr_ll refers to a different interfaces, it's ignored */
        if (sdlname == NULL || strcmp(sdlname, ifr->ifr_name) != 0)
            idx = hlen = 0;
        ifi->ifi_index = idx;
        ifi->ifi_hlen = hlen;
        if (ifi->ifi_hlen > IFI_HADDR)
            ifi->ifi_hlen = IFI_HADDR;
        if (hlen)
            memcpy(ifi->ifi_haddr, haddr, ifi->ifi_hlen);

        /* 17.7 ---------- */

        switch (ifr->ifr_addr.sa_family) {
            case AF_INET:
                sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
                ifi->ifi_addr = calloc(1, sizeof(struct sockaddr_in));
                memcpy(ifi->ifi_addr, sinptr, sizeof(struct sockaddr_in));

#ifdef SIOCGIFBRDADDR
                if (flags & IFF_BROADCAST) {
                    ioctl(sockfd, SIOCGIFBRDADDR, &ifrcopy);
                    sinptr = (struct sockaddr_in *) &ifrcopy.ifr_broadaddr;
                    ifi->ifi_brdaddr = calloc(1, sizeof(struct sockaddr_in));
                    memcpy(ifi->ifi_brdaddr, sinptr, sizeof(struct sockaddr_in));
                }
#endif

#if SIOCGIFDSTADDR
                if (flags & IFF_POINTOPOINT) {
                    ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
                    sinptr = (struct sockaddr_in *) &ifrcopy.ifr_dstaddr;
                    ifi->ifi_dstaddr = calloc(1, sizeof(struct sockaddr_in));
                    memcpy(ifi->ifi_dstaddr, sinptr, sizeof(struct sockaddr_in));
                }
#endif
                break;

            case AF_INET6:
                sin6ptr = (struct sockaddr_in6 *) &ifr->ifr_addr;
                ifi->ifi_addr = calloc(1, sizeof(struct sockaddr_in6));
                memcpy(ifi->ifi_addr, sin6ptr, sizeof(struct sockaddr_in6));

#if SIOCGIFDSTADDR
                if (flags & IFF_POINTOPOINT) {
                    ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
                    sin6ptr = (struct sockaddr_in6 *) &ifrcopy.ifr_dstaddr;
                    ifi->ifi_dstaddr = calloc(1, sizeof(struct sockaddr_in6));
                    memcpy(ifi->ifi_dstaddr, sin6ptr, sizeof(struct sockaddr_in6));
                }
#endif
                break;

            default:
                break;
        }
    }

    free(buf);
    return ifihead;     /* pointer to the head of linked list */
}

/* 17.8 -------- */

void free_ifi_info(struct ifi_info *ifihead)
{
    struct ifi_info *ifi, *ifinext;
    
    for (ifi = ifihead; ifi != NULL; ifi = ifinext) {
        if (ifi->ifi_addr != NULL)
            free(ifi->ifi_addr);
        if (ifi->ifi_brdaddr != NULL)
            free(ifi->ifi_brdaddr);
        if (ifi->ifi_dstaddr != NULL)
            free(ifi->ifi_dstaddr);
        /* current ifi_next will be gone after free() */
        ifinext = ifi->ifi_next;
    }
}
