#ifndef PACKETVECTOR_H
#define PACKETVECTOR_H

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <vector>

/**
 * @brief The PacketVector class
 */
class PacketVector : private std::vector<std::uint8_t /*, tbb::scalable_allocator<std::uint8_t>*/> {
public:
    typedef std::vector<std::uint8_t /*, tbb::scalable_allocator<std::uint8_t>*/> base;

    struct PacketMetadata {
        std::uint32_t UserId;
        std::uint16_t DataSize;
        std::uint64_t Random;
        std::uint16_t Reserved;

        bool HasIVCounter = false;
        std::uint64_t IVCounter;
    } PacketMetadata;

    static PacketVector *createPacketVector()
    {
        return new PacketVector();
    }

    static void deletePacketVector(PacketVector *p)
    {
        delete p;
    }

    std::uint8_t *packet()
    {
        return data() + _beginIdx;
    }

    const std::uint8_t *packet() const
    {
        return data() + _beginIdx;
    }

    std::size_t packetSize() const
    {
        return _endIdx - _beginIdx;
    }

    std::size_t underlyingDataSize() const
    {
        return size();
    }

    std::size_t beginIdx() const
    {
        return _beginIdx;
    }

    bool setBeginIdx(std::size_t beginIdx)
    {
        if (beginIdx < _endIdx) {
            _beginIdx = beginIdx;
            return true;
        } else {
            return false;
        }
    }

    std::size_t endIdx() const
    {
        return _endIdx;
    }

    bool setEndIdx(std::size_t endIdx)
    {
        if (endIdx > _beginIdx && endIdx <= size()) {
            _endIdx = endIdx;
            return true;
        } else {
            return false;
        }
    }

    void resetBeginEndIdx()
    {
        _beginIdx = 0;
        _endIdx = size();
    }

    bool setBeginEndIdx(std::size_t beginIdx, std::size_t endIdx)
    {
        if (endIdx > beginIdx && endIdx <= size()) {
            _beginIdx = beginIdx;
            _endIdx = endIdx;
            return true;
        } else {
            return false;
        }
    }

    bool grow(std::size_t size)
    {
        if (size > this->size()) {
            resize(size);
            return true;
        } else {
            return false;
        }
    }

    bool shiftData(std::size_t index, std::size_t size, int offset)
    {
        if (index + size + offset <= this->size() && index + offset >= 0) {
            std::memmove(data() + index + offset, data() + index, size);
            return true;
        } else {
            return false;
        }
    }

private:
    explicit PacketVector() {}

    explicit PacketVector(int size)
        : base(size)
        , _beginIdx(0)
        , _endIdx(size)
    {
    }

    explicit PacketVector(int size, std::uint8_t *data)
        : base(size)
        , _beginIdx(0)
        , _endIdx(size)
    {
        std::memcpy(this->data(), data, size);
    }

    PacketVector(const PacketVector &cpy)
        : base(cpy.size())
        , _beginIdx(0)
        , _endIdx(cpy.size())
    {
        std::memcpy(data(), cpy.data(), cpy.size());
    }

    //    const PacketVector &operator=(const PacketVector cpy)
    //    {
    //        this->resize(cpy.size());
    //        std::memcpy(data(), cpy.data(), cpy.size());
    //        return *this;
    //    }

    const PacketVector &operator=(const PacketVector cpy) = delete;

    std::size_t _beginIdx;
    std::size_t _endIdx;
};

#endif // PACKETVECTOR_H
