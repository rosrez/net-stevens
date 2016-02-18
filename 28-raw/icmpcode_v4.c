#include "trace.h"

const char *icmpcode_v4(int code)
{
    static char errbuf[100];
    switch (code) {
        case ICMP_NET_UNREACH:
            return ("network unreachable");
        case ICMP_HOST_UNREACH:
            return ("host unreachable");
        default:
            sprintf (errbuf, "[unknown code %d]", code);
            return errbuf;
    }
}
