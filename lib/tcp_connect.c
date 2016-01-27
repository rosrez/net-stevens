#include "unp.h"

int tcp_connect(const char *hostname, const char *service)
{
    int     sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    /* will get both IPv4 and IPv6 addresses */
    hints.ai_family = AF_UNSPEC;
    /* narrow down to TCP */
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(hostname, service, &hints, &res)) != 0)
        err_quit("tcp_connect error for %s, %s: %s",
                hostname, service, gai_strerror(n));

    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0)
            continue;       /* ignore this address */
        
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            break;

        close(sockfd);
    } while ((res = res->ai_next) != NULL);

    /* errno is set at the last call to connect() */
    if (res == NULL)
        err_sys("tcp_connect error for %s, %s", hostname, service);

    freeaddrinfo(ressave);

    return sockfd;
}
