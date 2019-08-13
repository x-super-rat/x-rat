#ifndef PACKETMANIPULATOR_H
#define PACKETMANIPULATOR_H

#include <cstdint>

#include <memory>
#include <vector>

#include "AbstractEncryptionProvider.h"
#include "AbstractKeyManager.h"
#include "CheesePacket.h"

class AbstractPacketManipulator {
public:
    virtual ~AbstractPacketManipulator() {}

    bool isPacketValid() { return _packetValid; }

    std::uint32_t getUserId() const;
    void setUserId(std::uint32_t userId);

    const std::uint8_t *getSalt() const;
    void getSalt(std::uint8_t *salt) const;
    void setSalt(std::uint8_t *salt);
    unsigned getSaltSize() const { return sizeof(CheeseHdr::Salt); }

    const std::uint8_t *getIV() const;
    void getIV(std::uint8_t *iv) const;
    void setIV(std::uint8_t *iv);
    unsigned getIVSize() const { return sizeof(CheeseHdr::DataSize); }

    unsigned setDataSize() const;
    void setDataSize(unsigned size);

    const std::uint8_t *getData() const;
    void getData(std::uint8_t *data) const;
    void setData(std::uint8_t *data, unsigned size) const;

    void appendRandomData();

protected:
    std::unique_ptr<AbstractKeyManager> _keyManager;
    std::unique_ptr<AbstractEncryptionProvider> _encryptionProvider;
    std::unique_ptr<std::uint8_t> _packet;
    bool _packetValid;
    int _packetSize;

    CheeseHdr *
    getHdr()
    {
        return reinterpret_cast<CheeseHdr *>(_packet.get());
    }

    const CheeseHdr *getHdr() const
    {
        return reinterpret_cast<const CheeseHdr *>(_packet.get());
    }

    bool packetCanGet() const
    {
        return _packetValid && _packet.operator bool();
    }

    bool packetCanSet() const
    {
        return _packet.operator bool();
    }
};

class PacketEncoder {
public:
    explicit PacketEncoder(AbstractKeyManager *keyManager, AbstractEncryptionProvider *encryptionProvider)
        : _keyManager(keyManager)
        , _encryptionProvider(encryptionProvider)
    {
    }

    virtual ~PacketEncoder() {}

    bool encodePacket(std::uint32_t userId, std::uint16_t dataSize, std::uint8_t *data);

protected:
    std::unique_ptr<AbstractKeyManager> _keyManager;
    std::unique_ptr<AbstractEncryptionProvider> _encryptionProvider;
};

class PacketManipulator {
public:
    explicit PacketManipulator(AbstractKeyManager *keyManager, AbstractEncryptionProvider *encryptionProvider)
        : _keyManager(keyManager)
        , _encryptionProvider(encryptionProvider)
        , _packet(nullptr)
        , _packetValid(false)
        , _packetSize(-1)
    {
    }

    virtual ~PacketManipulator() {}

    bool setPacket(const std::uint8_t *packet, unsigned size);
    bool isPacketValid() { return _packetValid; }

    std::uint32_t getUserId() const;
    void setUserId(std::uint32_t userId);

    const std::uint8_t *getSalt() const;
    void getSalt(std::uint8_t *salt) const;
    void setSalt(std::uint8_t *salt);
    unsigned getSaltSize() const { return sizeof(CheeseHdr::Salt); }

    const std::uint8_t *getIV() const;
    void getIV(std::uint8_t *iv) const;
    void setIV(std::uint8_t *iv);
    unsigned getIVSize() const { return sizeof(CheeseHdr::DataSize); }

    unsigned setDataSize() const;
    void setDataSize(unsigned size);

    const std::uint8_t *getData() const;
    void getData(std::uint8_t *data) const;
    void setData(std::uint8_t *data, unsigned size) const;

    void appendRandomData();

protected:
    std::unique_ptr<AbstractKeyManager> _keyManager;
    std::unique_ptr<AbstractEncryptionProvider> _encryptionProvider;
    std::unique_ptr<std::uint8_t> _packet;
    bool _packetValid;
    int _packetSize;

    CheeseHdr *
    getHdr()
    {
        return reinterpret_cast<CheeseHdr *>(_packet.get());
    }

    const CheeseHdr *getHdr() const
    {
        return reinterpret_cast<const CheeseHdr *>(_packet.get());
    }

    bool packetCanGet() const
    {
        return _packetValid && _packet.operator bool();
    }

    bool packetCanSet() const
    {
        return _packet.operator bool();
    }
};

#endif // PACKETMANIPULATOR_H
