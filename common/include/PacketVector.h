#ifndef PACKETVECTOR_H
#define PACKETVECTOR_H

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <vector>

#include <tbb/scalable_allocator.h>

class PacketVector : public std::vector<std::uint8_t, tbb::scalable_allocator<std::uint8_t>> {
public:
    typedef std::vector<std::uint8_t, tbb::scalable_allocator<std::uint8_t>> base;

    const PacketVector &operator=(const PacketVector cpy)
    {
        this->resize(cpy.size());
        std::memcpy(data(), cpy.data(), cpy.size());
        return *this;
    }

    struct PacketMetadata {
        std::uint32_t UserId;

    } PacketMetadata;

    template <typename falloc>
    static PacketVector *createPacketVector(falloc f = scalable_malloc)
    {
        return reinterpret_cast<PacketVector *>(f(sizeof(PacketVector)));
    }

    template <typename ffree>
    static void deletePacketVector(PacketVector *p, ffree f = scalable_free)
    {
        f(p);
    }

private:
    explicit PacketVector() {}

    explicit PacketVector(int size)
        : base(size)
    {
    }

    explicit PacketVector(int size, std::uint8_t *data)
        : base(size)
    {
        std::memcpy(this->data(), data, size);
    }

    PacketVector(const PacketVector &cpy)
        : base(cpy.size())
    {
        std::memcpy(data(), cpy.data(), cpy.size());
    }
};

#endif // PACKETVECTOR_H
