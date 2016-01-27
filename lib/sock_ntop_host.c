#include "unp.h"

char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
    static char str[128];   /* maximum length for UNIX domain socket name */

    switch (sa->sa_family) {
        case AF_INET: {
            struct sockaddr_in *sin = (struct sockaddr_in *) sa;

            if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                return NULL;
            return str;
        }

        case AF_INET6: {
            struct sockaddr_in6 *sin = (struct sockaddr_in6 *) sa;

            if (inet_ntop(AF_INET6, &sin->sin6_addr, str, sizeof(str)) == NULL)
                return NULL;
            return str;
        }
    }
}
