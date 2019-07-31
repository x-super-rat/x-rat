#include <arpa/inet.h>
#include <netinet/ip6.h>
#include <unistd.h>

#include "XRatServer.h"

int main(int argc, char *argv[])
{
    XRatServer srv(inet_addr("7.0.0.0"), 8);
    srv.initialize();
    srv.start();

    while (1) {
        sleep(60);
    }

    return 0;
}
