#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <syslog.h>

#include "x_rat_common.h"

int open_tun(const char *tun_name)
{
    ifreq ifr;
    int fd, err;
    const char *clonedev = "/dev/net/tun";

    if ((fd = open(clonedev, O_RDWR)) < 0) {
        syslog(LOG_ERR, "open /dev/net/tun failed: %s", std::strerror(errno));
        goto tun_alloc_err_open;
    }

    std::memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

    if (*tun_name) {
        std::strncpy(ifr.ifr_name, tun_name, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        syslog(LOG_CRIT, "ioctl tun failed: %s", std::strerror(err));
        goto tun_alloc_err_ioctl;
    }

    return fd;

tun_alloc_err_ioctl:
    close(fd);
tun_alloc_err_open:
    return -errno;
}
