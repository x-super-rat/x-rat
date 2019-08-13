#ifndef CHEESEPACKET_H
#define CHEESEPACKET_H

#include <cstdint>
#include <vector>

struct CheeseHdr {
    std::uint32_t UserId;
    std::uint8_t Salt[8];
    std::uint8_t IV[16];
    std::uint16_t DataSize;
} __attribute__((packed));

#endif // CHEESEPACKET_H
