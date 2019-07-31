#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include "Utilities.h"
#include "XRatServer.h"
#include "x_rat_common.h"

XRatServer::XRatServer(in_addr_t ipv4Pool, unsigned ipv4Prefix)
    : _tunFd(-1)
    , _linkFd(-1)
    , _connectionMap(ipv4Pool, ipv4Prefix, in6_addr(), 64)
    , _processThread(0)
{
}

bool XRatServer::initialize()
{
    const char *tun_name = "tun0";
    int tun_fd = open_tun(tun_name); /* tun interface */

    if (tun_fd < 0) {
        return false;
    }

    int link_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (link_fd < 0) {
        perror("socket");
        return false;
    }

    int on = 1;
    int rc = ioctl(link_fd, FIONBIO, reinterpret_cast<char *>(&on));
    if (rc < 0) {
        perror("ioctl() sock failed");
        close(link_fd);
        return false;
    }

    rc = ioctl(tun_fd, FIONBIO, reinterpret_cast<char *>(&on));
    if (rc < 0) {
        perror("ioctl() tun failed");
        close(tun_fd);
        return false;
    }

    struct sockaddr_in6 localaddr = {};

    localaddr.sin6_family = AF_INET6;
    localaddr.sin6_addr = IN6ADDR_ANY_INIT;
    localaddr.sin6_port = htons(1221);

    // Bind the socket with the server address
    if (::bind(link_fd, (const struct sockaddr *)&localaddr, sizeof(localaddr)) < 0) {
        perror("bind failed");
        close(tun_fd);
        close(link_fd);
        return false;
    }

    _linkFd = link_fd;
    _tunFd = tun_fd;
    return true;
}

bool XRatServer::start()
{
    // FIXME: check if already started
    _processThread = pthread_create(&_processThread, nullptr, [](void *obj) -> void * {
        auto srv = static_cast<XRatServer *>(obj);
        srv->processLoop();
        return nullptr;
    },
        this);

    return true;
}

int XRatServer::processIncomingInternetPacket()
{
    char buffer[2048];
    int nread = read(_tunFd, buffer, sizeof(buffer));
    printf("TUN: read %d bytes\n", nread);

    if (nread < 0) {
        perror("Reading from interface");
        return 1;
    }

    ip *hdrv4 = reinterpret_cast<ip *>(buffer);
    OriginalSourceConnectionInfo original;
    TransformedSourceConnectionInfo xfrm;

    if (hdrv4->ip_v == 4) {
        xfrm.xfrm_src_address = Utilities::map4to6(hdrv4->ip_dst);
        bool exist = _connectionMap.getOriginal(xfrm, original);
        if (!exist)
            return -1; // Not found
        hdrv4->ip_dst = Utilities::map6to4(original.src_address);
        hdrv4->ip_sum = 0;
        hdrv4->ip_sum = Utilities::ipChecksum(hdrv4, sizeof(ip));
    } else if (hdrv4->ip_v == 6) {
        ip6_hdr *hdrv6 = reinterpret_cast<ip6_hdr *>(buffer);
        xfrm.xfrm_src_address = hdrv6->ip6_dst;
        bool exist = _connectionMap.getOriginal(xfrm, original);
        if (!exist)
            return -1; // Not found
        hdrv6->ip6_dst = original.src_address;
    } else {
        return -1; // wth the protocol is it?
    }

    sockaddr_in6 remoteaddr = {};
    remoteaddr.sin6_addr = original.client_address;
    remoteaddr.sin6_port = original.client_port;
    remoteaddr.sin6_family = AF_INET6;

    sendto(_linkFd, buffer, nread, MSG_CONFIRM, (sockaddr *)&remoteaddr, sizeof(remoteaddr));

    return 0;
}

// Packet from client
int XRatServer::processIncomingClientPacket()
{
    char buffer[2048];

    sockaddr_in6 ratclientaddr = {};

    unsigned int len = sizeof(ratclientaddr);
    int nread = recvfrom(_linkFd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&ratclientaddr, &len);

    if (static_cast<unsigned int>(nread) < sizeof(ip)) {
        fprintf(stderr, "Invalid packet, too small: %d bytes\n", nread);
        return 2;
    }

    OriginalSourceConnectionInfo original;

    original.client_address = ratclientaddr.sin6_addr;
    original.client_port = ratclientaddr.sin6_port;

    ip *hdrv4 = reinterpret_cast<ip *>(buffer);
    if (hdrv4->ip_v == 0x04) {
        original.proto = hdrv4->ip_p;
        original.src_address = Utilities::map4to6(hdrv4->ip_src);

        TransformedSourceConnectionInfo xfrm;
        _connectionMap.getTransformed(original, xfrm);

        hdrv4->ip_src = Utilities::map6to4(xfrm.xfrm_src_address);
        hdrv4->ip_sum = 0;
        hdrv4->ip_sum = Utilities::ipChecksum(hdrv4, sizeof(ip));

    } else if (hdrv4->ip_v == 0x06) {
        ip6_hdr *hdrv6 = reinterpret_cast<ip6_hdr *>(buffer);
        original.proto = hdrv6->ip6_nxt;
        original.src_address = hdrv6->ip6_src;

        TransformedSourceConnectionInfo xfrm;
        _connectionMap.getTransformed(original, xfrm);

        hdrv6->ip6_src = xfrm.xfrm_src_address;
    } else {
        return -1; // bad proto type
    }

    write(_tunFd, buffer, nread);
    printf("packet %d bytes\n", nread);

    return 0;
}

int XRatServer::processLoop()
{
    const int nfds = 2;
    pollfd fds[nfds] = {
        { .fd = _tunFd,
            .events = POLLIN,
            .revents = 0 },
        { .fd = _linkFd,
            .events = POLLIN,
            .revents = 0 }
    };

    while (true) {
        int rc = poll(fds, 2, -1);
        if (rc < 0) {
            perror("  poll() failed");
            return -1;
        }

        int current_size = nfds;
        for (int i = 0; i < current_size; i++) {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].revents != POLLIN) {
                printf("  Error! revents = %d\n", fds[i].revents);
                // TODO: check POLLERR
            }

            if (fds[i].fd == _tunFd) {
                processIncomingInternetPacket();
            }

            if (fds[i].fd == _linkFd) {
                processIncomingClientPacket();
            }
        }
    }
    return 0;
}
