#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <string.h>

#include <limits.h>
#include "mmdb.h"

#define PFX_DB "%s/.local/share/ipnetdb/latest/ipnetdb_prefix_latest.mmdb"
#define ASN_DB "%s/.local/share/ipnetdb/latest/ipnetdb_asn_latest.mmdb"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s key...\n", argv[0]);
        return 1;
    }
    char pfx_db_path[PATH_MAX];
    char asn_db_path[PATH_MAX];
    snprintf(pfx_db_path, PATH_MAX, PFX_DB, getenv("HOME"));
    snprintf(asn_db_path, PATH_MAX, ASN_DB, getenv("HOME"));
    mmdb_t *pfx_db = mmdb_open(pfx_db_path);
    mmdb_t *asn_db = mmdb_open(asn_db_path);

    if (pfx_db == NULL || asn_db == NULL) {
        fprintf(stderr, "Failed to open DBs\n");
        return 1;
    }

    struct in6_addr bip6;
    struct in_addr bip4;

    for (int i = 1; i < argc; i++) {
        mmdb_type_t *lr = NULL;
        size_t len = strlen(argv[1]);
        if (strspn(argv[1], ".0123456789") == len && inet_pton(AF_INET, argv[i], &bip4))
            lr = mmdb_lookup4(pfx_db, (uint8_t *)&bip4);
        else if (strspn(argv[1], ":0123456789abcdefABCDEF") == len && inet_pton(AF_INET6, argv[i], &bip6))
            lr = mmdb_lookup6(pfx_db, (uint8_t *)&bip6);
        else {
            char *end;
            uint32_t asn = htonl(strtol(argv[i], &end, 10));
            if (asn > 0 && end == (argv[i] + len))
                lr = mmdb_lookup4(asn_db, (uint8_t *)&asn);
        }
        if (lr) {
            mmdb_print(lr);
            putchar('\n');
            mmdb_type_free(lr);
        } else {
            printf("{\"error\": \"lookup failed for %s\"}\n", argv[i]);
        }
    }
    mmdb_close(pfx_db);
    mmdb_close(asn_db);
    return 0;
}
