#include "unp.h"
#include <time.h>

/* 
 * This function will output time with microsecond precision,
 * in the following format:
 *
 * 12:34:56.123456
 */

char *gf_time(void)
{
    struct timeval tv;
    static char str[30];
    char *ptr;

    if (gettimeofday(&tv, NULL) < 0)
        err_sys("gettimeofday() error");

    ptr = ctime(&tv.tv_sec);
    strcpy(str, &ptr[11]);
    /* Fri Sep 11 00:00:00 1986\n\0 */
    /* 012345678901234567890123 4 5 */
    snprintf(str + 8, sizeof(str) - 8, ".%06ld", tv.tv_usec);
    return str;
}
