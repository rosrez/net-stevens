#ifndef __GWUTIL_H__
#define __GWUTIL_H__

#include <stdint.h>

/* --- Defines --- */

#define GW_INTF_LEN 32

/* --- Types --- */

typedef struct gateway_info {
    char            interface[GW_INTF_LEN];
    uint32_t        ip;
    unsigned char   mac[6];
} gateway_info;

/* --- Function prototypes --- */

int gateway_enum(gateway_info *gw, int capacity);
void gateway_fprint(FILE *f, gateway_info *gw);
int gateway_get_mac(gateway_info *gw, int timeout);

#endif /* __GWUTIL_H__ */
