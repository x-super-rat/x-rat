#ifndef UTILITIES_TEST_H
#define UTILITIES_TEST_H

#include <arpa/inet.h>
#include <netinet/in.h>

#include <gtest/gtest.h>

#include "Utilities.h"

TEST(Utilities, CIDR_Match_Test)
{
    in_addr a, b, c, network, network2, networkdef;
    a.s_addr = inet_addr("10.0.0.1");
    b.s_addr = inet_addr("10.200.200.200");
    c.s_addr = inet_addr("7.0.0.01");
    network.s_addr = inet_addr("10.0.0.0");
    network2.s_addr = inet_addr("10.200.200.0");
    networkdef.s_addr = inet_addr("0.0.0.0");

    ASSERT_TRUE(Utilities::matchCIDR(network, a, 8));
    ASSERT_TRUE(Utilities::matchCIDR(network, a, 16));
    ASSERT_TRUE(Utilities::matchCIDR(network, a, 24));
    ASSERT_FALSE(Utilities::matchCIDR(network, a, 32));
    ASSERT_TRUE(Utilities::matchCIDR(networkdef, a, 0));

    ASSERT_TRUE(Utilities::matchCIDR(network, b, 8));
    ASSERT_FALSE(Utilities::matchCIDR(network, b, 16));
    ASSERT_FALSE(Utilities::matchCIDR(network, b, 24));
    ASSERT_TRUE(Utilities::matchCIDR(network2, b, 24));
    ASSERT_TRUE(Utilities::matchCIDR(networkdef, b, 0));

    ASSERT_FALSE(Utilities::matchCIDR(network, c, 8));
    ASSERT_TRUE(Utilities::matchCIDR(networkdef, b, 0));
}

#endif
