#ifndef IPV6BITMAP_H
#define IPV6BITMAP_H

#include <netinet/ip.h>

class IPv6BitMap {
public:
    explicit IPv6BitMap(in_addr ipv6PoolAddress, in_addr ipv6PoolPrefix);

    int allocate(in6_addr &ip, in_port_t &port);
    int allocate(in6_addr *ip, in_port_t *port);
    int deallocate(const in6_addr ip, const in_port_t port);
};

#endif // IPV6BITMAP_H
