#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdint>
#include <cstring>

#include <netinet/ip6.h>

class Utilities {
public:
    static bool isv4Mapped(const in6_addr &addr)
    {
        return IN6_IS_ADDR_V4MAPPED(&addr);
    }

    static in_addr map6to4(const in6_addr &addr)
    {
        in_addr v4;
        std::memcpy(&v4.s_addr, reinterpret_cast<const std::uint8_t *>(&addr) + 12, 4);
        return v4;
    }

    static in6_addr map4to6(const in_addr &addr)
    {
        in6_addr mapped = {};
        mapped.s6_addr32[2] = htonl(0xffff);

        std::memcpy(reinterpret_cast<std::uint8_t *>(&mapped.s6_addr) + 12, &addr.s_addr, 4);
        return mapped;
    }

    static unsigned int countLeadingZero(std::uint64_t data)
    {
#if defined(__LP64__)
        return __builtin_clzl(data);
#elif defined(__ILP32__)
        return __builtin_clzll(data);
#else
#error "Unsupported data model"
#endif
    }

    static unsigned int countLeadingZero(std::uint32_t data)
    {
#if defined(__LP64__) || defined(__ILP32__)
        return __builtin_clz(data);
#else
#error "Unsupported data model"
#endif
    }

    static unsigned int findFirstSet(std::uint64_t data)
    {
        int firstSet = 0;
#if defined(__LP64__) || defined(__ILP32__)
        firstSet = ffsl(data);
#elif defined(__ILP64__)
        firstSet = ffs(data);
#elif defined(__LLP64)
        firstSet = ffsll(data);
#else
#error "Unsupported data model"
#endif
        return firstSet;
    }

    static bool matchCIDR(const in_addr &network, const in_addr &addr, unsigned prefix);
    static bool matchCIDR(const in_addr_t &network, const in_addr_t &addr, unsigned prefix);
    static bool validateCIDR(const in_addr &network, unsigned prefix);
    static bool validateCIDR(const in_addr_t &network, unsigned prefix);
    static std::uint16_t ipChecksum(void *vdata, std::size_t length);

private:
    static const std::uint32_t CIDR_PREFIXES[33];
};

#endif // UTILITIES_H
