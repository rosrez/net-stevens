#include <stdlib.h>
#include <stdio.h>

#include "gwutil.h"

#define GWINFO_CAPACITY 8

int main(int argc, char **argv)
{
    gateway_info gw[GWINFO_CAPACITY];
    int i, cnt;

    if ((cnt = gateway_enum(&gw[0], GWINFO_CAPACITY)) == -1) {
        fprintf(stderr, "Could not obtain gateway info from the routing table\n");
        exit(EXIT_FAILURE);
    }

    if (cnt == 0) {
        fprintf(stderr, "Found no gateway info in the routing table\n");
        exit(EXIT_SUCCESS);
    }

    for (i = 0; i < cnt; i++) { 
        gateway_fprint(stdout, &gw[i]); 
        gateway_get_mac(&gw[i], 1000000);
        gateway_fprint(stdout, &gw[i]);
    }

    exit(EXIT_SUCCESS);
}
