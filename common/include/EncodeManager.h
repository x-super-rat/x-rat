#ifndef ENCODEMANAGER_H
#define ENCODEMANAGER_H

#include <cstdint>

#include "PacketVector.h"

class EncodeManager {
public:
    explicit EncodeManager();

    bool encode(PacketVector *packet);
    bool decode(PacketVector *packet);

private:
    std::uint32_t getUserId(const PacketVector *packet);
    std::uint64_t getRandom(const PacketVector *packet);
    std::uint16_t getDataSize(const PacketVector *packet);

    void fillPacketHeader(PacketVector *packet);
    void fillPacketIVCounter(PacketVector *packet);
};

#endif // ENCODEMANAGER_H
