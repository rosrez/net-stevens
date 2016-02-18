#include "trace.h"

const char *icmpcode_v4(int code)
{
    static char errbuf[100];
    switch (code) {
        case ICMP_DST_UNREACH_NOROUTE:
            return ("no route to host");
        case ICMP_DST_UNREACH_ADMIN:
            return ("administratively prohibited");
        case ICMP_DST_UNREACH_NOTNEIGHBOR:
            return ("not a neighbor");
        case ICMP_DST_UNREACH_ADDR:
            return ("address unreachable");
        case ICMP_DST_UNREACH_NOPORT:
            return ("port unreachable");
        default:
            sprintf (errbuf, "[unknown code %d]", code);
            return errbuf;
    }
}
