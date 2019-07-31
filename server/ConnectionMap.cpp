#include "ConnectionMap.h"
#include "Utilities.h"

ConnectionMap::ConnectionMap(in_addr_t ipv4PoolAddress, unsigned ipv4PoolPrefix,
    in6_addr ipv6PoolAddress, unsigned ipv6PoolPrefix)
    : _ipv4BitMap(ipv4PoolAddress, ipv4PoolPrefix)
{
}

bool ConnectionMap::getTransformed(OriginalSourceConnectionInfo info, TransformedSourceConnectionInfo &xfrm)
{
    bool isIPv4 = Utilities::isv4Mapped(info.src_address);
    std::lock_guard<std::mutex> lock(_lock);

    auto it = _connectionMap.left.find(info);
    if (it == _connectionMap.left.end()) {
        TransformedSourceConnectionInfo xfrm;

        int rc;
        if (isIPv4) {
            in_addr ip;
            rc = _ipv4BitMap.allocate(ip);
            if (rc != IPv4BitMap::STATUS_OK)
                return false;
            xfrm.xfrm_src_address = Utilities::map4to6(ip);
            _connectionMap.left.insert({ info, xfrm });
        } else {
            // FIXME: IPv6 support
            return false;
        }
    } else {
        xfrm = it->get_right();
    }
    return true;
}

bool ConnectionMap::getOriginal(TransformedSourceConnectionInfo info, OriginalSourceConnectionInfo &original)
{
    // FIXME: check IPv6
    auto it = _connectionMap.right.find(info);
    if (it == _connectionMap.right.end()) {
        return false;
    } else {
        original = it->get_left();
        return true;
    }
}
