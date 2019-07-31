#ifndef X_RAT_COMMON_H
#define X_RAT_COMMON_H

#include <cstdint>

#include <netinet/ip6.h>

int open_tun(const char *tun_name);
std::uint16_t ip_checksum(void *vdata, std::size_t length);

inline bool operator==(const in_addr &a, const in_addr &b)
{
    return a.s_addr == b.s_addr;
}

#endif // X_RAT_COMMON_H
