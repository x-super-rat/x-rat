#ifndef XRATSERVER_H
#define XRATSERVER_H

#include <pthread.h>

#include "ConnectionMap.h"
#include "IPv4BitMap.h"

class XRatServer {
public:
    explicit XRatServer(in_addr_t ipv4Pool, unsigned ipv4Prefix);

    bool initialize();
    bool start();
    bool stop();

private:
    int _tunFd;
    int _linkFd;
    ConnectionMap _connectionMap;
    pthread_t _processThread;

    int processIncomingInternetPacket();
    int processIncomingClientPacket();
    int processLoop();
};

#endif // XRATSERVER_H
