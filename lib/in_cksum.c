#include "unp.h"

/*
 * Stevens, 28.15.
 * Calculate the Internet checksum.
 *
 * The first while loop calculates the sum of all the 16-bit values.
 * If the length is odd, then the final byte is added in with the sum.
 */

uint16_t in_cksum(uint16_t *addr, int len)
{
    int     nleft = len;
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;

    /* 
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up odd byte, if necessary */
    if (nleft == 1) {
        *(unsigned char *) (&answer) = *(unsigned char *) w;
        sum += answer;
    }

    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);     /* add high 16 to low 16 */
    sum += (sum >> 16);                     /* add carry */
    answer = ~sum;                          /* (negate &) truncate to 16 bits */
    return answer;
}
