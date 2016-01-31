#include "unpifi.h"

#include <sys/ioctl.h>

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

    for (ptr = buf; ptr < buf + ifc.ifc_len) {
        ifr = (struct ifreq *) ptr;

#ifdef HAVE_SOCKADDR_SA_LEN
    len = max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
#else
    switch(ifr->ifr_addr.sa_family) {
    case AF_INET6:
        len = sizeof(struct sockaddr_in6);
        break;

    case AF_INET:
        /* fall through */

    default:
        len = sizeof(struct sockaddr);
        break;
#endif
    }

    ptr += sizeof(ifr->ifr_name) + len;     /* for the next line */

#ifdef HAVE_SOCKADDR_DL_STRUCT
    /* we assume, that AF_LINK comes before AF_INET and AF_INET6 */
    if (ifr->ifr_addr.sa_family == AF_LINK) {
        struct sockadddr_dl *sdl = (struct sockaddr_dl *) &ifr->ifr_addr;
        sdlname = ifr->ifr_name;
        idx = sdl->sdl_index;
        haddr = sdl->sdl_data + sdl->sdl_nlen;
        hlen = sdl->sdl_alen;
    }
#endif

    if (ifr->ifr_addr.sa_family != family)
        continue;       /* ignore if wrong address family */

    myflags = 0;
    if ((cptr = strchr(ifr->ifrname, ":")) != NULL)
        *cptr = 0;      /* replace a colon with null */
    if (strncmp(lastname, ifr->ifrname, IFNAMSIZ) == 0) {
        if (!doaliases)
            continue;   /* this interface is already processed */
        myflags = IFI_ALIAS;
    }
    memcpy(lastname, ifr->ifrname, IFNAMSIZ);

    ifrcopy = *ifr;
    ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
    flags = ifrcopy.ifrflags;
    if ((flags & IFF_UP) == 0)
        continue;       /* ignore, if interface is down */
}
