#include "unp.h"

int sock_bind_wild(int sockfd, int family) 
{
    return 0;
}

/* Returns:
 * 0 if addresses are of the same family and are equal, -1 otherwise */

int sock_cmp_addr(const struct sockaddr *sockaddr1, const struct sockaddr *sockaddr2, socklen_t addrlen)
{
    if (sockaddr1->sa_family != sockaddr2->sa_family)
        return -1;

    switch (sockaddr1->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sa1 = (struct sockaddr_in *) sockaddr1;
        struct sockaddr_in *sa2 = (struct sockaddr_in *) sockaddr2;
        if (memcmp((void *) &sa1->sin_addr, (void *) &sa2->sin_addr, sizeof(sa1->sin_addr)) == 0)
            return 0;
        else
            return 1;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sa1 = (struct sockaddr_in6 *) sockaddr1;
        struct sockaddr_in6 *sa2 = (struct sockaddr_in6 *) sockaddr2;
        if (memcmp((void *) &sa1->sin6_addr, (void *) &sa2->sin6_addr, sizeof(sa1->sin6_addr)) == 0)
            return 0;
        else
            return 1;

    }

    default:
        return -1;
    }
}

/* Returns:
 * 0 if addresses are of the same family and ports are equal, -1 otherwise */

int sock_cmp_port(const struct sockaddr *sockaddr1, const struct sockaddr *sockaddr2, socklen_t addrlen)
{
    if (sockaddr1->sa_family != sockaddr2->sa_family)
        return -1;

    switch (sockaddr1->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sa1 = (struct sockaddr_in *) sockaddr1;
        struct sockaddr_in *sa2 = (struct sockaddr_in *) sockaddr2;
        return (sa1->sin_port == sa2->sin_port) ? 0 : -1;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sa1 = (struct sockaddr_in6 *) sockaddr1;
        struct sockaddr_in6 *sa2 = (struct sockaddr_in6 *) sockaddr2;
        return (sa1->sin6_port == sa2->sin6_port) ? 0 : -1;
    }

    default:
        return -1;
    }
}

/* Returns:
 * nonnegative port number for IPv4 and IPv6 addresses, -1 otherwise */

int sock_get_port(const struct sockaddr *sockaddr, socklen_t addrlen)
{
    switch (sockaddr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sa = (struct sockaddr_in *) sockaddr;
        return sa->sin_port;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sa = (struct sockaddr_in6 *) sockaddr;
        return sa->sin6_port;
    }

    default:
        return -1;
    }
}

void sock_set_addr(const struct sockaddr *sockaddr, socklen_t addrlen, void *ptr)
{
    switch (sockaddr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sa = (struct sockaddr_in *) sockaddr;
        memcpy((void *) &sa->sin_addr, ptr, sizeof(sa->sin_addr) - 1);
        return;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sa = (struct sockaddr_in6 *) sockaddr;
        memcpy((void *) &sa->sin6_addr, ptr, sizeof(sa->sin6_addr));
        return;
    }
    }
}

/* Assumes that port is in network byte order */

void sock_set_port(const struct sockaddr *sockaddr, socklen_t addrlen, int port)
{
    switch (sockaddr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sa = (struct sockaddr_in *) sockaddr;
        sa->sin_port = port;
        return;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sa = (struct sockaddr_in6 *) sockaddr;
        sa->sin6_port = port;
        return;
    }
    }
}
