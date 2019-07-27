#include <cstring>
#include <net/if.h>

#include "x-rat-client-main.h"

int main()
{
    char tun_name[IFNAMSIZ];

    /* Connect to the device */
    strcpy(tun_name, "tun77");
    int tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI); /* tun interface */

    if (tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }

    /* Now read data coming from the kernel */
    while (1) {
        /* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
        nread = read(tun_fd, buffer, sizeof(buffer));
        if (nread < 0) {
            perror("Reading from interface");
            close(tun_fd);
            exit(1);
        }

        /* Do whatever with the data */
        printf("Read %d bytes from device %s\n", nread, tun_name);
    }

    cout << "Hello World!" << endl;
    return 0;
}
