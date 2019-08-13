#include <cstring>

#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <sys/random.h>

#include "CheesePacket.h"
#include "PacketManipulator.h"

bool PacketManipulator::setPacket(const std::uint8_t *packet, unsigned size)
{
    _packet.reset();

    if (size < sizeof(CheeseHdr) + sizeof(ip)) {
        _packetValid = false;
        return false;
    }

    _packet.reset(new std::uint8_t[size]);
    std::memcpy(_packet.get(), packet, size);
    return true;
}

std::uint32_t PacketManipulator::getUserId() const
{
    if (!packetCanGet())
        return 0;

    std::uint32_t userIdBE = getHdr()->UserId;
    return ntohl(userIdBE);
}

void PacketManipulator::setUserId(std::uint32_t userId)
{
    if (!packetCanSet())
        return;

    getHdr()->UserId = htonl(userId);
}

const uint8_t *PacketManipulator::getSalt() const
{
    if (!packetCanGet())
        return nullptr;

    return getHdr()->Salt;
}

void PacketManipulator::getSalt(uint8_t *salt) const
{
    std::memcpy(salt, getSalt(), getSaltSize());
}

void PacketManipulator::setSalt(uint8_t *salt)
{
    if (!packetCanSet())
        return;

    std::memcpy(getHdr()->Salt, salt, getSaltSize());
}

const uint8_t *PacketManipulator::getIV() const
{
    if (!packetCanGet())
        return nullptr;

    return getHdr()->IV;
}

void PacketManipulator::getIV(uint8_t *iv) const
{
    std::memcpy(iv, getHdr()->IV, getIVSize());
}

void PacketManipulator::setIV(uint8_t *iv)
{
    if (!packetCanSet())
        return;

    std::memcpy(getHdr()->IV, iv, getIVSize());
}

unsigned PacketManipulator::setDataSize() const
{
    if (!packetCanGet())
        return 0;

    return getHdr()->DataSize;
}

void PacketManipulator::setDataSize(unsigned size)
{
    if (!packetCanSet())
        return;

    getHdr()->DataSize = size;
}

bool PacketEncoder::encodePacket(std::uint32_t userId, std::uint16_t dataSize, std::uint8_t *data)
{
    constexpr int cheeseHdrSize = sizeof(CheeseHdr);
    std::uint8_t random[256 + 1];
    std::uint8_t IV[sizeof(CheeseHdr::IV)];
    getrandom(random, sizeof(random), 0);
    getrandom(IV, sizeof(IV), 0);
    const int randomTrailSize = random[0]; // u int 8 has max of 255
    const int payloadSize = dataSize + 16 + randomTrailSize;
    int packetSize = cheeseHdrSize + payloadSize;

    std::vector<std::uint8_t> packet(packetSize);

    std::vector<std::uint8_t> salt;
    std::vector<std::uint8_t> key;
    _encryptionProvider->encryptStatic(key.data(), key.size(), IV, sizeof(IV), data, dataSize, packet.data() + cheeseHdrSize);

    auto hdr = reinterpret_cast<CheeseHdr *>(packet.data());
    hdr->UserId = htonl(userId);
    std::memcpy(&hdr->Salt, salt.data(), sizeof(CheeseHdr::Salt));
    std::memcpy(&hdr->IV, IV, sizeof(CheeseHdr::IV));
    hdr->DataSize = htons(dataSize);

    for (int i = 0; i < cheeseHdrSize; ++i) {
        *(reinterpret_cast<std::uint8_t *>(packet.data()) + i) ^= packet[i];
    }

    constexpr int blockSize = 16;
    int encryptedSize = dataSize % blockSize == 0 ? dataSize : dataSize + (blockSize - (dataSize % blockSize));

    std::memcpy(packet.data() + sizeof(CheeseHdr) + encryptedSize, random + 1, randomTrailSize);

    return true;
}
