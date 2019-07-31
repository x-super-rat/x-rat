#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <cstdint>
#include <cstring>

#include <netinet/ip6.h>

struct OriginalSourceConnectionInfo {
    in6_addr client_address;
    in_port_t client_port;
    in6_addr src_address;
    //    in_port_t src_port;
    std::uint8_t proto; // Protocol for IPv4, Next Header for IPv6
};

struct TransformedSourceConnectionInfo {
    in6_addr xfrm_src_address;
    //    in_port_t xfrm_src_port;
};

inline bool operator<(const OriginalSourceConnectionInfo &a, const OriginalSourceConnectionInfo &b)
{
    int rc;

    rc = std::memcmp(reinterpret_cast<const void *>(&a.client_address),
        reinterpret_cast<const void *>(&b.client_address),
        sizeof(in6_addr));
    if (rc < 0)
        return true;
    if (rc > 0)
        return false;

    rc = (int)a.client_port - (int)b.client_port;
    if (rc < 0)
        return true;
    if (rc > 0)
        return false;

    rc = std::memcmp(reinterpret_cast<const void *>(&a.src_address),
        reinterpret_cast<const void *>(&b.src_address),
        sizeof(in6_addr));
    if (rc < 0)
        return true;
    if (rc > 0)
        return false;

    //    rc = (int)a.src_port - (int)b.src_port;
    //    if (rc < 0)
    //        return true;
    //    if (rc > 0)
    //        return false;

    rc = (int)a.proto - (int)b.proto;
    return rc < 0;
}

inline bool operator<(const TransformedSourceConnectionInfo &a, const TransformedSourceConnectionInfo &b)
{
    int rc;

    rc = std::memcmp(reinterpret_cast<const void *>(&a.xfrm_src_address),
        reinterpret_cast<const void *>(&b.xfrm_src_address),
        sizeof(in6_addr));
    if (rc < 0)
        return true;
    if (rc > 0)
        return false;

    //    rc = (int)a.xfrm_src_port - (int)b.xfrm_src_port;
    //    return rc < 0;
    return false;
}

#endif // CONNECTIONINFO_H
