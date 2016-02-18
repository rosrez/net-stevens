#include "trace.h"

/* 
 * Stevens, 28.19
 *
 * MAIN LOOP; SET HOP LIMIT AND SEND THREE PROBES
 * The main loop of the function is a double nested for loop. The outer loop starts 
 * the TTL or hop limit at 1, and increases it by 1, while the inner loop sends three probes
 * (UDP datagrams) to the destination. Each time the TTL changes, we call setsockopt() to set
 * the new value using either the IP_TTL or IPV6_UNICAST_HOPS socket option.
 *
 * Each time around the outer loop, we initialize the socket address structure pointed to by
 * salast to 0. This structure will be compared to the socket address structure returned by
 * recvfrom() when the ICMP message is read, and if the two structures are different,
 * the IP address from the new structure will be printed. Using this technique, the IP address
 * corresponding to the first probe for each TTL is printed, and should the IP address change
 * for a given value of the TTL (say, a route changes while we are running the program),
 * the new IP address will then be printed.
 *
 * SET DESTINATION PORT AND SEND UDP DATAGRAM
 * Each time a probe packet is sent, the destination port in the sasend socket address structure
 * is changed by calling our sock_set_port() function. The reason for changing the port 
 * for each probe is that when we reach the final destination, all three probes are sent to
 * a different port, and hopefully at least one of the ports is not in use.
 *
 * READ ICMP MESSAGE
 * One of our functions, recv_v4() or recv_v6(), calls recvfrom() to read and process
 * the returned ICMP messages. These two functions return -3 if a timeout occurs (telling us
 * to send another probe if we haven't sent three for this TTL), -2 if an ICMP
 * "time exceeded in transit" error is received, -1 if an ICMP "port unreachable" error is
 * received (which means we have reached the final destination), or the non-negative
 * ICMP code if some other ICMP destination unreachable error is received.
 *
 * PRINT REPLY
 * If this is the first reply for a given TTL, of if the IP address of the node sending
 * the ICMP message has changed for this TTL, we print the hostname and IP address, or
 * just the IP address (if the call to getnameinfo() doesn't return the hostname).
 * The RTT is calculated as the time difference from when we sent the probe to the time when
 * the ICMP message was returned and printed.
 */

void traceloop(void)
{
    int     seq, code, done;
    double  rtt;
    struct rec *rec;
    struct timeval tvrecv;
    struct timeval to = {.tv_sec = 1};

    recvfd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
    setuid(getuid());               /* don't need special permissions anymore */

    /* 
     * Linux doesn't seem to interrupt the recvfrom() system call. We use 
     * recvfrom() on our raw receiving socket to collect ICMP responses.
     * To prevent recv_v[46]() routines from waiting forever, we set a 
     * 1-second timeout on the socket.
     *
     * Another technique we could employ is to call select() with a timeout
     * and make sure we have something to read prior to calling recvfrom().
     */
    Setsockopt(recvfd, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof(to));

    if (pr->sasend->sa_family == AF_INET6 && verbose == 0) {
        struct icmp6_filter myfilt;
        ICMP6_FILTER_SETBLOCKALL(&myfilt);
        ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &myfilt);
        ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &myfilt);
        setsockopt(recvfd, IPPROTO_IPV6, ICMP6_FILTER, &myfilt, sizeof(myfilt));
    }
    
    sendfd = Socket(pr->sasend->sa_family, SOCK_DGRAM, 0);

    pr->sabind->sa_family = pr->sasend->sa_family;
    sport = (getpid() & 0xffff) | 0x8000;               /* our source UDP port # */
    sock_set_port(pr->sabind, pr->salen, htons(sport));
    Bind(sendfd, pr->sabind, pr->salen);

    sig_alrm(SIGALRM);

    seq = 0;
    done = 0;
    for (ttl = 1; ttl <= max_ttl && done == 0; ttl++) {
        Setsockopt(sendfd, pr->ttllevel, pr->ttloptname, &ttl, sizeof(int));
        bzero(pr->salast, pr->salen);

        printf("%2d ", ttl);
        fflush(stdout);

        for (probe = 0; probe < nprobes; probe++) {
            rec = (struct rec *) sendbuf;
            rec->rec_seq = ++seq;
            rec->rec_ttl = ttl;
            gettimeofday(&rec->rec_tv, NULL);

            sock_set_port(pr->sasend, pr->salen, htons(dport + seq));
            Sendto(sendfd, sendbuf, datalen, 0, pr->sasend, pr->salen);

            if ((code = (*pr->recv)(seq, &tvrecv)) == -3) {
                printf(" *");       /* timeout, no reply */
            } else {
                char    str[NI_MAXHOST];

                if (sock_cmp_addr(pr->sarecv, pr->salast, pr->salen) != 0) {
                    if (getnameinfo(pr->sarecv, pr->salen, str, sizeof(str), NULL, 0, 0) == 0)
                        printf(" %s (%s)", str, sock_ntop_host(pr->sarecv, pr->salen));
                    else
                        printf(" %s", sock_ntop_host(pr->sarecv, pr->salen));
                    memcpy(pr->salast, pr->sarecv, pr->salen);
                }

                tv_sub(&tvrecv, &rec->rec_tv);
                rtt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
                printf(" %.3f ms", rtt);

                if (code == -1)     /* port unreachable; at destination */
                    done++;
                else if (code >= 0)
                    printf(" (ICMP %s)", (pr->icmpcode)(code));

            }
            fflush(stdout);
        }
        printf("\n");
    }
}
