#ifndef CHEESEPACKET_H
#define CHEESEPACKET_H

#include <cstdint>
#include <vector>

//struct CheeseHdr {
//    std::uint32_t UserId;
//    std::uint8_t Salt[8];
//    std::uint8_t IV[16];
//    std::uint16_t DataSize;
//} __attribute__((packed));

enum {
    CHEESE_USER_ID_OFFSET = 0,
    CHEESE_RANDOM_DATA_OFFSET = 4,
    CHEESE_DATA_SIZE_OFFSET = 12,
    CHEESE_RESERVED_OFFSET = 14,
    CHEESE_PAYLOAD_OFFSET = 16,

    CHEESE_HEADER_SIZE = 16,
};

#endif // CHEESEPACKET_H
