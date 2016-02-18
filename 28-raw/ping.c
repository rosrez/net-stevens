#include "ping.h"

struct proto proto_v4 = 
        { proc_v4, send_v4, NULL, NULL, NULL, 0, IPPROTO_ICMP };

struct proto proto_v6 = 
        { proc_v6, send_v6, NULL, NULL, NULL, 0, IPPROTO_ICMPV6 };

int datalen = 56;       /* data that goes with ICMP echo request */

int main(int argc, char *argv[])
{
    int     c;
    struct addrinfo *ai;
    char    *h;

    opterr = 0;         /* don't want getopt() writing to stderr */
    while ((c = getopt(argc, argv, "v")) != -1) {
        switch (c) {
        case 'v':
            verbose++;
            break;
        case '?':
            err_quit("unrecognized option: %c", c);
        }
    }

    if (optind != argc - 1)
        err_quit("Usage: ping [-v] <hostname>");
    host = argv[optind];

    pid = getpid() & 0xffff;      /* ICMP ID field is 16 bits */
    signal(SIGALRM, sig_alrm);

    /* call the host_serv() wrapper around getaddrinfo() */
    if ((ai = host_serv(host, NULL, 0, 0)) == NULL)
        err_quit("can't get address info for %s\n", host);

    h = sock_ntop_host(ai->ai_addr, ai->ai_addrlen);
    printf("PING %s (%s): %d data bytes\n",
            ai->ai_canonname ? ai->ai_canonname : h, h, datalen);

    /* initialize according to protocol */
    if (ai->ai_family == AF_INET) {
        pr = &proto_v4;
    } else if (ai->ai_family == AF_INET6) {
        pr = &proto_v6;
        if (IN6_IS_ADDR_V4MAPPED(&(((struct sockaddr_in6 *) ai->ai_addr)->sin6_addr)))
                err_quit("cannot ping IPv4-mapped IPv6 address");
    } else {
        err_quit("unknown address family %d", ai->ai_family);
    }

    pr->sasend = ai->ai_addr;
    pr->sarecv = calloc(1, ai->ai_addrlen);
    pr->salen = ai->ai_addrlen;

    readloop();

    exit(0);
}
