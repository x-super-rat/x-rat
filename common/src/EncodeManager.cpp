#include "EncodeManager.h"
#include "CheesePacket.h"
#include "RandomPool.h"
#include "Utilities.h"

bool EncodeManager::encode(PacketVector *packet)
{
    // 1. get randoms
    std::uint8_t trailingDataSize;

    auto rp = RandomPool::instance();
    rp->fill(&trailingDataSize);
    rp->fill(&packet->PacketMetadata.Random);
    rp->fill(&packet->PacketMetadata.Reserved);

    // 2. calculate size
    std::size_t originalPacketSize = packet->underlyingDataSize();
    std::size_t size = originalPacketSize + CHEESE_HEADER_SIZE + trailingDataSize
        + (packet->PacketMetadata.HasIVCounter ? sizeof(PacketVector::PacketMetadata::IVCounter) : 0);

    packet->grow(size);
    packet->resetBeginEndIdx();
    packet->shiftData(0, originalPacketSize, CHEESE_HEADER_SIZE);
    fillPacketHeader(packet);
    fillPacketIVCounter(packet);
    rp->fill(packet->packet() + originalPacketSize + CHEESE_HEADER_SIZE);

    return true;
}

bool EncodeManager::decode(PacketVector *packet)
{
    if (packet->packetSize() < 32) {
        return false;
    }

    packet->PacketMetadata.UserId = getUserId(packet);
    packet->PacketMetadata.Random = getRandom(packet);
    packet->PacketMetadata.DataSize = getDataSize(packet);
    packet->setBeginIdx(CHEESE_PAYLOAD_OFFSET);
    return true;
}

std::uint32_t EncodeManager::getUserId(const PacketVector *packet)
{
    const std::uint32_t *beuid = reinterpret_cast<const std::uint32_t *>(packet->packet());
    const std::uint32_t *uidxor = reinterpret_cast<const std::uint32_t *>(packet->packet() + CHEESE_PAYLOAD_OFFSET);
    return be32toh(*beuid ^ *uidxor);
}

std::uint64_t EncodeManager::getRandom(const PacketVector *packet)
{
    const std::uint64_t *random = reinterpret_cast<const std::uint64_t *>(packet->packet() + CHEESE_RANDOM_DATA_OFFSET);
    return be64toh(*random);
}

std::uint16_t EncodeManager::getDataSize(const PacketVector *packet)
{
    const std::uint16_t *bedatasize = reinterpret_cast<const std::uint16_t *>(packet->packet() + CHEESE_DATA_SIZE_OFFSET);
    const std::uint16_t *bedsxor = reinterpret_cast<const std::uint16_t *>(packet->packet() + CHEESE_RESERVED_OFFSET);
    return be16toh(*bedatasize ^ *bedsxor);
}

void EncodeManager::fillPacketHeader(PacketVector *packet)
{
    std::uint64_t *berandom = reinterpret_cast<std::uint64_t *>(packet->packet() + CHEESE_RANDOM_DATA_OFFSET);
    *berandom = htobe64(packet->PacketMetadata.Random);

    std::uint16_t *bereserved = reinterpret_cast<std::uint16_t *>(packet->packet() + CHEESE_RESERVED_OFFSET);
    *bereserved = htobe16(packet->PacketMetadata.Reserved);

    std::uint32_t *beuid = reinterpret_cast<std::uint32_t *>(packet->packet());
    std::uint32_t *uidxor = reinterpret_cast<std::uint32_t *>(packet->packet() + CHEESE_PAYLOAD_OFFSET);
    *beuid = htobe32(packet->PacketMetadata.UserId ^ *uidxor);

    std::uint16_t *bedatasize = reinterpret_cast<std::uint16_t *>(packet->packet() + CHEESE_DATA_SIZE_OFFSET);
    *bedatasize = htobe16(packet->PacketMetadata.DataSize ^ packet->PacketMetadata.Reserved);
}

void EncodeManager::fillPacketIVCounter(PacketVector *packet)
{
    if (packet->PacketMetadata.HasIVCounter) {
        std::uint32_t *bectr = reinterpret_cast<std::uint32_t *>(packet->packet() + (packet->packetSize() - 4));
        std::uint32_t *ctrxor = reinterpret_cast<std::uint32_t *>(packet->packet() + CHEESE_RANDOM_DATA_OFFSET);
        *bectr = htobe32(packet->PacketMetadata.IVCounter & *ctrxor);
    }
}
