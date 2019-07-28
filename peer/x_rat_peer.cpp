#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include "x_rat_common.h"

int process_incoming_tun_packet(int tun_fd, int link_fd)
{
    char buffer[2048];
    int nread = read(tun_fd, buffer, sizeof(buffer));

    if (nread < 0) {
        perror("Reading from interface");
        return 1;
    }

    write(link_fd, buffer, nread);

    printf("TUN: read %d bytes\n", nread);
    return 0;
}

int process_incoming_link_packet(int link_fd, int tun_fd)
{
    char buffer[2048];
    int nread = read(link_fd, buffer, sizeof(buffer));

    if (nread < 0) {
        perror("Reading from interface");
        return 1;
    }

    write(tun_fd, buffer, nread);

    printf("Link: read %d bytes\n", nread);
    return 0;
}

int main(int argc, char *argv[])
{
    const char *tun_name = "tun0";
    int tun_fd = open_tun(tun_name); /* tun interface */

    if (tun_fd < 0) {
        exit(1);
    }

    int link_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (link_fd < 0) {
        perror("socket");
        exit(1);
    }

    int on = 1;
    int off = 0;
    int rc = setsockopt(link_fd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&off, sizeof(off));
    if (rc < 0) {
        perror("setsockopt");
        close(link_fd);
        exit(1);
    }

    rc = ioctl(link_fd, FIONBIO, reinterpret_cast<char *>(&on));
    if (rc < 0) {
        perror("ioctl() sock failed");
        close(link_fd);
        exit(-1);
    }

    rc = ioctl(tun_fd, FIONBIO, reinterpret_cast<char *>(&on));
    if (rc < 0) {
        perror("ioctl() tun failed");
        close(tun_fd);
        exit(-1);
    }

    struct sockaddr_in6 localaddr = {};
    localaddr.sin6_family = AF_INET6;
    localaddr.sin6_addr = IN6ADDR_ANY_INIT;
    localaddr.sin6_port = htons(1221);

    if (bind(link_fd, (const struct sockaddr *)&localaddr, sizeof(localaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in6 remoteaddr = {};
    remoteaddr.sin6_family = AF_INET6;
    rc = inet_pton(AF_INET6, "::FFFF:169.254.0.2", &remoteaddr.sin6_addr);
    if (rc != 1) {
        perror("inet_pton");
        exit(-1);
    }
    remoteaddr.sin6_port = htons(1221);

    rc = connect(link_fd, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr));
    if (rc < 0) {
        perror("connect");
        exit(1);
    }

    const int nfds = 2;
    pollfd fds[nfds] = {
        { .fd = tun_fd,
            .events = POLLIN,
            .revents = 0 },
        { .fd = link_fd,
            .events = POLLIN,
            .revents = 0 }
    };

    bool end_server = 0;

    while (true) {
        rc = poll(fds, 2, -1);
        if (rc < 0) {
            perror("  poll() failed");
            break;
        }

        int current_size = nfds;
        for (int i = 0; i < current_size; i++) {
            if (fds[i].revents == 0)
                continue;

            if (fds[i].revents != POLLIN) {
                printf("  Error! revents = %d on fd no. %d\n", fds[i].revents, i);

                int error;
                socklen_t errlen = sizeof(error);
                getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, &error, &errlen);
                printf("error=%d, msg=%s\n", error, strerror(error));
                end_server = true;
                break;
            }

            if (fds[i].fd == tun_fd) {
                process_incoming_tun_packet(tun_fd, link_fd);
                continue;
            }

            if (fds[i].fd == link_fd) {
                process_incoming_link_packet(link_fd, tun_fd);
                continue;
            }
        }
    }

    return 0;
}
