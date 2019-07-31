#ifndef IPV4BITMAP_H
#define IPV4BITMAP_H

#include <cstdint>

#include <atomic>
#include <map>
#include <mutex>
#include <vector>

#include <netinet/ip.h>

class IPv4BitMap {
public:
    explicit IPv4BitMap(in_addr_t ipv4PoolAddress, unsigned prefix);

    int allocate(in_addr &ip);
    int allocate(in_addr *ip);
    int deallocate(const in_addr ip);

    bool getIsNetworkCIDRValid() const
    {
        return _isNetworkCIDRValid;
    }

    int getAllocatedEntries() const
    {
        return _allocatedEntries;
    }

    std::uint32_t getMaxEntries() const
    {
        return _maxEntries;
    }

    enum StatusCode : int {
        STATUS_OK = 0,
        STATUS_INVALID_POOL = -1,
        STATUS_ADDRESS_DRAINED = -2,
    };

private:
    std::vector<std::uint64_t> _ipPortBitMapL1; // bit: idx of ip_offset * (65536-64-4096) = ip_offset * 61376
    std::vector<std::uint64_t> _ipPortBitMapL2; // map per 64 L1 entry, 1 if corresponding L1 entries are all 0xFFFFFFFFFFFFFFFF

    std::size_t _lastAllocatedIPPortMapL2Idx = 0;

    in_addr_t _network;
    std::uint32_t _prefix;
    std::uint64_t _maxEntries;
    std::atomic_uint _allocatedEntries;
    bool _isNetworkCIDRValid;
    static constexpr std::uint64_t FULL_BITMAP = ~0;

    std::mutex _lock;

    unsigned long allocateBitMap();
    void clearIPPortBitMapL2Bit(std::size_t l1Idx);
    void setIPPortBitMapL2BitByL1No(std::size_t l1Idx);
    void rebuildIPPortBitMapL2(std::size_t l1Idx);
    void rebuildIPPortBitMapL2();
    std::uint32_t getLocalEndiannessNetworkBaseAddr() const;

    void setIPPortBitMapL2BitByL2No(std::size_t l2BitNo, bool isFull);
};

#endif // IPV4BITMAP_H
