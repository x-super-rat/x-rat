// <BEGIN LICENSE>
/*************************************************************************
 *
 * Meow Home Tech LLC CONFIDENTIAL
 * __________________
 *
 *  [2018] Meow Home Tech LLC
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains the property of
 * Meow Home Tech LLC and its suppliers, if any. The intellectual and
 * technical concepts contained herein are proprietary to Meow Home Tech LLC
 * and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law.
 *
 * Dissemination of this information or reproduction of this material is strictly
 * forbidden unless prior written permission is obtained from Meow Home Tech LLC.
 *
 * If you do not receive explicit consent of accessing this information from
 * Meow Home Tech, please close this file immediately. Unauthorized review, use,
 * retention, and/or distribution is strictly prohibited.
 **************************************************************************/
// <END LICENSE>

#include <iostream>

using namespace std;

int main()
{
    char tun_name[IFNAMSIZ];

    /* Connect to the device */
    strcpy(tun_name, "tun77");
    tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI); /* tun interface */

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
