#ifndef PACKETVECTOR_H
#define PACKETVECTOR_H

#include <cstdint>
#include <cstring>

#include <vector>

#include <tbb/scalable_allocator.h>

class PacketVector : std::vector<std::uint8_t, tbb::scalable_allocator<std::uint8_t>> {
public:
    typedef std::vector<std::uint8_t, tbb::scalable_allocator<std::uint8_t>> base;

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

    const PacketVector &operator=(const PacketVector cpy)
    {
        this->resize(cpy.size());
        std::memcpy(data(), cpy.data(), cpy.size());
        return *this;
    }
};

#endif // PACKETVECTOR_H
