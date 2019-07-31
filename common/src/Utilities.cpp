#include "Utilities.h"

const std::uint32_t Utilities::CIDR_PREFIXES[33] = {
    [0] = htonl(0),
    [1] = htonl(0x80000000),
    [2] = htonl(0xC0000000),
    [3] = htonl(0xE0000000),
    [4] = htonl(0xF0000000),
    [5] = htonl(0xF8000000),
    [6] = htonl(0xFC000000),
    [7] = htonl(0xFE000000),
    [8] = htonl(0xFF000000),
    [9] = htonl(0xFF800000),
    [10] = htonl(0xFFC00000),
    [11] = htonl(0xFFE00000),
    [12] = htonl(0xFFF00000),
    [13] = htonl(0xFFF80000),
    [14] = htonl(0xFFFC0000),
    [15] = htonl(0xFFFE0000),
    [16] = htonl(0xFFFF0000),
    [17] = htonl(0xFFFF8000),
    [18] = htonl(0xFFFFC000),
    [19] = htonl(0xFFFFE000),
    [20] = htonl(0xFFFFF000),
    [21] = htonl(0xFFFFF800),
    [22] = htonl(0xFFFFFC00),
    [23] = htonl(0xFFFFFE00),
    [24] = htonl(0xFFFFFF00),
    [25] = htonl(0xFFFFFF80),
    [26] = htonl(0xFFFFFFC0),
    [27] = htonl(0xFFFFFFE0),
    [28] = htonl(0xFFFFFFF0),
    [29] = htonl(0xFFFFFFF8),
    [30] = htonl(0xFFFFFFFC),
    [31] = htonl(0xFFFFFFFE),
    [32] = htonl(0xFFFFFFFF),
};

bool Utilities::matchCIDR(const in_addr &network, const in_addr &addr, unsigned prefix)
{
    return matchCIDR(network.s_addr, addr.s_addr, prefix);
}

bool Utilities::matchCIDR(const in_addr_t &network, const in_addr_t &addr, unsigned prefix)
{
    if (prefix > 32)
        return false;

    std::uint32_t cidr = CIDR_PREFIXES[prefix];
    std::uint32_t hNetwork = network;
    std::uint32_t hAddr = addr;

    return hNetwork == (hAddr & cidr);
}

bool Utilities::validateCIDR(const in_addr &network, unsigned prefix)
{
    return validateCIDR(network.s_addr, prefix);
}

bool Utilities::validateCIDR(const in_addr_t &network, unsigned prefix)
{
    if (prefix > 32)
        return false;

    std::uint32_t cidr = ~CIDR_PREFIXES[prefix];
    std::uint32_t hAddr = network;

    if ((hAddr & cidr) == 0)
        return true;
    else
        return false;
}

uint16_t Utilities::ipChecksum(void *vdata, std::size_t length)
{
    // Cast the data pointer to one that can be indexed.
    char *data = (char *)vdata;

    // Initialise the accumulator.
    std::uint64_t acc = 0xffff;

    // Handle any partial block at the start of the data.
    std::size_t offset = ((std::uintptr_t)data) & 3;
    if (offset) {
        std::size_t count = 4 - offset;
        if (count > length)
            count = length;
        std::uint32_t word = 0;
        std::memcpy(offset + (char *)&word, data, count);
        acc += ntohl(word);
        data += count;
        length -= count;
    }

    // Handle any complete 32-bit blocks.
    char *data_end = data + (length & ~3);
    while (data != data_end) {
        std::uint32_t word;
        std::memcpy(&word, data, 4);
        acc += ntohl(word);
        data += 4;
    }
    length &= 3;

    // Handle any partial block at the end of the data.
    if (length) {
        std::uint32_t word = 0;
        memcpy(&word, data, length);
        acc += ntohl(word);
    }

    // Handle deferred carries.
    acc = (acc & 0xffffffff) + (acc >> 32);
    while (acc >> 16) {
        acc = (acc & 0xffff) + (acc >> 16);
    }

    // If the data began at an odd byte address
    // then reverse the byte order to compensate.
    if (offset & 1) {
        acc = ((acc & 0xff00) >> 8) | ((acc & 0x00ff) << 8);
    }

    // Return the checksum in network byte order.
    return htons(~acc);
}
