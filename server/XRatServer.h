#ifndef XRATSERVER_H
#define XRATSERVER_H

#include <memory>
#include <thread>

#include <uv.h>

#include "ConnectionMap.h"
#include "IPv4BitMap.h"

class XRatServer {
public:
    explicit XRatServer(in_addr_t ipv4Pool, unsigned ipv4Prefix);

    bool initialize();
    bool start();
    bool stop();

private:
    uv_loop_t _uvloop;
    uv_udp_t _linkSock;
    //    uv__t _linkSock;

    int _tunFd;
    int _linkFd;
    ConnectionMap _connectionMap;
    std::unique_ptr<std::thread> _processThread;

    int processIncomingInternetPacket();
    int processIncomingClientPacket(uv_udp_t *handle, ssize_t nread, const uv_buf_t *rcvbuf, const struct sockaddr *addr, unsigned flags);
    int processLoop();
};

#endif // XRATSERVER_H
