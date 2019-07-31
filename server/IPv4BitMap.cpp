#include <unistd.h>

#include "IPv4BitMap.h"
#include "Utilities.h"

IPv4BitMap::IPv4BitMap(in_addr_t ipv4PoolAddress, unsigned prefix)
    : _network(ipv4PoolAddress)
    , _prefix(prefix)
    , _allocatedEntries(0)
{
    _isNetworkCIDRValid = prefix <= 30;
    if (!_isNetworkCIDRValid)
        return;

    _isNetworkCIDRValid = Utilities::validateCIDR(ipv4PoolAddress, prefix);
    if (!_isNetworkCIDRValid)
        return;

    _maxEntries = ((std::uint64_t)1 << (32 - prefix)) - 3;
}

int IPv4BitMap::allocate(in_addr &ip)
{
    if (!_isNetworkCIDRValid)
        return STATUS_INVALID_POOL;

    std::lock_guard<std::mutex> lock(_lock);

    if (_allocatedEntries.load() >= _maxEntries) {
        return STATUS_ADDRESS_DRAINED; // Not enough IP
    }

    unsigned long ipIdx = allocateBitMap();

    std::uint32_t baseAddr = getLocalEndiannessNetworkBaseAddr();
    in_addr_t addr;
    addr = htonl(baseAddr + ipIdx);
    ip.s_addr = addr;

    _allocatedEntries.fetch_add(1);

    return STATUS_OK;
}

int IPv4BitMap::allocate(in_addr *ip)
{
    return allocate(*ip);
}

int IPv4BitMap::deallocate(const in_addr ip)
{
    if (!_isNetworkCIDRValid)
        return STATUS_INVALID_POOL;

    std::lock_guard<std::mutex> lock(_lock);
    std::uint32_t l1bitIdx = ntohl(ip.s_addr) - getLocalEndiannessNetworkBaseAddr();
    std::size_t l1ElementIdx = l1bitIdx / 64;
    l1bitIdx %= 64;

    std::uint64_t &l1Entry = _ipPortBitMapL1[l1ElementIdx];
    if (!(l1Entry & (std::uint64_t)1 << l1bitIdx)) { // Not set yet, not allocated
        return STATUS_OK;
    }

    l1Entry &= ~((std::uint64_t)1 << l1bitIdx);
    _allocatedEntries.fetch_sub(1);

    rebuildIPPortBitMapL2(l1ElementIdx);
    return STATUS_OK;
}

unsigned long IPv4BitMap::allocateBitMap()
{
    long l2first0bitIdx = -1;

    // Search L2 for unfilled bitmap idx
    for (std::size_t i = _lastAllocatedIPPortMapL2Idx; i < _ipPortBitMapL2.size(); ++i) {
        const std::uint64_t l2entry = _ipPortBitMapL2.at(i);

        if (l2entry == FULL_BITMAP) // This bitmap is full.
            continue;

        const std::uint64_t invertedMapL2 = ~l2entry;
        auto first0 = Utilities::findFirstSet(invertedMapL2);
        // find the least significant bit 0 idx
        // first0 is index of first bit 0 *plus 1*
        // == 0 means no bit 0 found

        if (first0 == 0) {
            throw; // This shouldn't happen since not full is verified before
        } else {
            // -1 to actual bit of 0
            --first0;

            l2first0bitIdx = i * 64 + first0;
            _lastAllocatedIPPortMapL2Idx = i;
            break;
        }
    }

    if (l2first0bitIdx == -1) { // All bitmaps are full
        _ipPortBitMapL2.push_back(0);
        l2first0bitIdx = (_ipPortBitMapL2.size() - 1) * 64;
    }

    std::size_t l1IdxStart = l2first0bitIdx * 64;
    std::size_t l1IdxEnd = l1IdxStart + 64; // exclusive

    if (_ipPortBitMapL1.size() < l1IdxEnd) {
        _ipPortBitMapL1.resize(l1IdxEnd);
    }

    unsigned long allocatedBitIdx = 0;
    bool allocated = false;
    for (std::size_t i = l1IdxStart; i < l1IdxEnd; ++i) {
        std::uint64_t &l1Entry = _ipPortBitMapL1[i];
        if (l1Entry == FULL_BITMAP)
            continue;

        const std::uint64_t invertedL1Entry = ~l1Entry;
        auto first0 = Utilities::findFirstSet(invertedL1Entry);
        if (first0 == 0) {
            throw;
        } else {
            --first0;
            l1Entry |= (std::uint64_t)1 << first0; // Set bit;
            allocatedBitIdx = i * 64 + first0;
            allocated = true;

            if (l1Entry == FULL_BITMAP) {
                rebuildIPPortBitMapL2(i);
            }

            break;
        }
    }

    if (!allocated) { // L1 entries are all full. Check rebuild L2
        throw;
    }

    return allocatedBitIdx;
}

void IPv4BitMap::clearIPPortBitMapL2Bit(std::size_t l1Idx)
{
    if (_ipPortBitMapL1.size() < l1Idx + 1) // Invalid size
        throw;

    std::size_t l2BitIdx = l1Idx / 64; // This bit corresponds to range of 64 l1Idx
    setIPPortBitMapL2BitByL2No(l2BitIdx, false);
}

void IPv4BitMap::setIPPortBitMapL2BitByL1No(std::size_t l1Idx)
{
    if (_ipPortBitMapL1.size() < l1Idx + 1) // Invalid size
        throw;

    std::size_t l2BitIdx = l1Idx / 64; // This bit corresponds to range of 64 l1Idx
    setIPPortBitMapL2BitByL2No(l2BitIdx, true);
}

void IPv4BitMap::rebuildIPPortBitMapL2(std::size_t l1ElementIdx)
{
    if (_ipPortBitMapL1.size() < l1ElementIdx + 1) // Invalid size
        throw;

    std::size_t l2BitIdx = l1ElementIdx / 64; // This bit corresponds to range of 64 l1Idx

    bool isFull = true;
    for (std::size_t i = l2BitIdx * 64; i < l2BitIdx * 64 + 64; ++i) {
        if (_ipPortBitMapL1.at(i) != FULL_BITMAP) {
            isFull = false;
            break;
        }
    }

    setIPPortBitMapL2BitByL2No(l2BitIdx, isFull);
}

void IPv4BitMap::rebuildIPPortBitMapL2()
{
    // FIXME: add code
    throw;
}

uint32_t IPv4BitMap::getLocalEndiannessNetworkBaseAddr() const
{
    return ntohl(_network) + 1 + 1; // +1 for first address in network, +1 for tun IP
}

void IPv4BitMap::setIPPortBitMapL2BitByL2No(std::size_t l2BitNo, bool isFull)
{
    std::size_t l2ElementIdx = l2BitNo / 64;
    unsigned l2BitIdx = l2BitNo % 64;
    if (_ipPortBitMapL2.size() < l2ElementIdx + 1) {
        throw; // This is not happening
    }
    std::uint64_t &l2Bitmap = _ipPortBitMapL2[l2ElementIdx];

    if (isFull) {
        l2Bitmap |= (std::uint64_t)1 << l2BitIdx;
    } else {
        l2Bitmap &= ~((std::uint64_t)1 << l2BitIdx);
        if (l2ElementIdx < _lastAllocatedIPPortMapL2Idx) {
            _lastAllocatedIPPortMapL2Idx = l2ElementIdx;
        }
    }
}
