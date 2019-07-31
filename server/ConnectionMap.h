#ifndef CONNECTIONMAP_H
#define CONNECTIONMAP_H

#include <mutex>

#include <boost/bimap.hpp>

#include "ConnectionInfo.h"
#include "IPv4BitMap.h"

class ConnectionMap {
public:
    explicit ConnectionMap(in_addr_t ipv4PoolAddress, unsigned ipv4PoolPrefix,
        in6_addr ipv6PoolAddress, unsigned ipv6PoolPrefix);

    bool getTransformed(OriginalSourceConnectionInfo info, TransformedSourceConnectionInfo &xfrm);
    bool getOriginal(TransformedSourceConnectionInfo info, OriginalSourceConnectionInfo &original);

private:
    boost::bimap<OriginalSourceConnectionInfo, TransformedSourceConnectionInfo> _connectionMap;
    IPv4BitMap _ipv4BitMap;

    std::mutex _lock;
};

#endif // CONNECTIONMAP_H
