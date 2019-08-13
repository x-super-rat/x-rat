#ifndef PACKETDECODER_H
#define PACKETDECODER_H

#include <cstdint>

class PacketDecoder {
public:
    explicit PacketDecoder();
    void setPacket(const std::uint8_t *data, unsigned int size);
    bool decode();
    bool isValid();

    const char *getIV();

private:
    const std::uint8_t *_data;
    bool _isValid;
};

#endif // PACKETDECODER_H
