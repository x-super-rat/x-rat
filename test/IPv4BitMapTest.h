#ifndef IPV4_BITMAP_TEST_H
#define IPV4_BITMAP_TEST_H

#include <arpa/inet.h>

#include <gtest/gtest.h>

#include "IPv4BitMap.h"
#include "x_rat_common.h"

TEST(IPv4BitMapTest, Allocation)
{
    IPv4BitMap bitmap(inet_addr("7.0.0.0"), 8);

    std::vector<in_addr> allocated;
    allocated.reserve(20);
    for (unsigned i = 0; i < 20; ++i) {
        in_addr ip;
        int rc = bitmap.allocate(ip);
        ASSERT_EQ(rc, 0);
        allocated.push_back(ip);
    }

    const char *ips[] = {
        "7.0.0.2",
        "7.0.0.3",
        "7.0.0.4",
        "7.0.0.5",
        "7.0.0.6",
        "7.0.0.7",
        "7.0.0.8",
        "7.0.0.9",
        "7.0.0.10",
        "7.0.0.11",
        "7.0.0.12",
        "7.0.0.13",
        "7.0.0.14",
        "7.0.0.15",
        "7.0.0.16",
        "7.0.0.17",
        "7.0.0.18",
        "7.0.0.19",
        "7.0.0.20",
        "7.0.0.21",
    };

    for (unsigned i = 0; i < 20; ++i) {
        ASSERT_EQ(allocated.at(i).s_addr, inet_addr(ips[i]));
    }
}

TEST(IPv4BitMapTest, Dellocation)
{
    IPv4BitMap bitmap(inet_addr("7.0.0.0"), 8);

    std::vector<in_addr> allocated;
    allocated.reserve(20);
    for (unsigned i = 0; i < 20; ++i) {
        in_addr ip;
        int rc = bitmap.allocate(ip);
        ASSERT_EQ(rc, 0);
        allocated.push_back(ip);
    }

    for (auto ip : allocated) {
        bitmap.deallocate(ip);
    }

    in_addr ip;
    int rc = bitmap.allocate(ip);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(ip.s_addr, inet_addr("7.0.0.2"));
}

TEST(IPv4BitMapTest, RandomDellocation)
{
    IPv4BitMap bitmap(inet_addr("7.0.0.0"), 8);

    std::vector<in_addr> allocated;

    allocated.reserve(20);
    for (unsigned i = 0; i < 20; ++i) {
        in_addr ip;
        int rc = bitmap.allocate(ip);
        ASSERT_EQ(rc, 0);
        allocated.push_back(ip);
    }

    int rand1 = rand() / (double)RAND_MAX * allocated.size();
    int rand2 = rand() / (double)RAND_MAX * allocated.size();

    auto dealloc1 = allocated[rand1];
    bitmap.deallocate(dealloc1);

    auto dealloc2 = allocated[rand2];
    bitmap.deallocate(dealloc2);

    in_addr ip;

    if (ntohl(dealloc1.s_addr) > ntohl(dealloc2.s_addr)) {
        in_addr tmp = dealloc1;
        dealloc1 = dealloc2;
        dealloc2 = tmp;
    }

    int rc = bitmap.allocate(ip);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(ip, dealloc1);

    rc = bitmap.allocate(ip);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(ip, dealloc2);
}

TEST(IPv4BitMapTest, DrainAddressPool)
{
    IPv4BitMap bitmap(inet_addr("192.168.0.0"), 24);

    std::vector<in_addr> allocated;
    allocated.reserve(256);
    for (unsigned i = 0; i < 253; ++i) { // From 2 to 254
        in_addr ip;
        int rc = bitmap.allocate(ip);
        ASSERT_EQ(rc, 0);
        allocated.push_back(ip);
    }

    in_addr ip;
    int rc = bitmap.allocate(ip);
    ASSERT_NE(rc, 0); // 255
}

#endif // IPV4_BITMAP_TEST_H
