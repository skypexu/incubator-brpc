// Copyright (c) 2014 Baidu, Inc.
//
// Author: Ge,Jun (gejun@baidu.com)
// Date: 2010-12-04 11:59

#include <gtest/gtest.h>
#include "butil/errno.h"
#include "butil/endpoint.h"
#include "butil/logging.h"

namespace {

TEST(EndPointTest, comparisons) {
    butil::EndPoint p1(butil::int2ip(1234), 5678);
    butil::EndPoint p2 = p1;
    ASSERT_TRUE(p1 == p2 && !(p1 != p2));
    ASSERT_TRUE(p1 <= p2 && p1 >= p2 && !(p1 < p2 || p1 > p2));
    ++p2.port;
    ASSERT_TRUE(p1 != p2 && !(p1 == p2));
    ASSERT_TRUE(p1 < p2 && p2 > p1 && !(p2 <= p1 || p1 >= p2));
    --p2.port;
    p2.ip = butil::int2ip(butil::ip2int(p2.ip)-1);
    ASSERT_TRUE(p1 != p2 && !(p1 == p2));
    ASSERT_TRUE(p1 > p2 && p2 < p1 && !(p1 <= p2 || p2 >= p1));
}

TEST(EndPointTest, ip_t) {
    LOG(INFO) << "INET_ADDRSTRLEN = " << INET_ADDRSTRLEN;
    
    butil::ip_t ip0;
    ASSERT_EQ(0, butil::str2ip("1.1.1.1", &ip0));
    ASSERT_STREQ("1.1.1.1", butil::ip2str(ip0).c_str());
    ASSERT_EQ(-1, butil::str2ip("301.1.1.1", &ip0));
    ASSERT_EQ(-1, butil::str2ip("1.-1.1.1", &ip0));
    ASSERT_EQ(-1, butil::str2ip("1.1.-101.1", &ip0));
    ASSERT_STREQ("1.0.0.0", butil::ip2str(butil::int2ip(1)).c_str());

    butil::ip_t ip1, ip2, ip3;
    ASSERT_EQ(0, butil::str2ip("192.168.0.1", &ip1));
    ASSERT_EQ(0, butil::str2ip("192.168.0.2", &ip2));
    ip3 = ip1;
    ASSERT_LT(ip1, ip2);
    ASSERT_LE(ip1, ip2);
    ASSERT_GT(ip2, ip1);
    ASSERT_GE(ip2, ip1);
    ASSERT_TRUE(ip1 != ip2);
    ASSERT_FALSE(ip1 == ip2);
    ASSERT_TRUE(ip1 == ip3);
    ASSERT_FALSE(ip1 != ip3);
}

TEST(EndPointTest, show_local_info) {
    LOG(INFO) << "my_ip is " << butil::my_ip() << std::endl
              << "my_ip_cstr is " << butil::my_ip_cstr() << std::endl
              << "my_hostname is " << butil::my_hostname();
}

TEST(EndPointTest, endpoint) {
    butil::EndPoint p1;
    ASSERT_EQ(butil::IP_ANY, p1.ip);
    ASSERT_EQ(0, p1.port);
    ASSERT_FALSE(butil::is_unix_sock_endpoint(p1));
    
    butil::EndPoint p2(butil::IP_NONE, -1);
    ASSERT_EQ(butil::IP_NONE, p2.ip);
    ASSERT_EQ(-1, p2.port);
    ASSERT_FALSE(butil::is_unix_sock_endpoint(p2));

    butil::EndPoint p3;
    ASSERT_EQ(-1, butil::str2endpoint(" 127.0.0.1:-1", &p3));
    ASSERT_EQ(-1, butil::str2endpoint(" 127.0.0.1:65536", &p3));
    ASSERT_EQ(0, butil::str2endpoint(" 127.0.0.1:65535", &p3));
    ASSERT_EQ(0, butil::str2endpoint(" 127.0.0.1:0", &p3));
    ASSERT_FALSE(butil::is_unix_sock_endpoint(p3));

    butil::EndPoint p4;
    ASSERT_EQ(0, butil::str2endpoint(" 127.0.0.1: 289 ", &p4));
    ASSERT_STREQ("127.0.0.1", butil::ip2str(p4.ip).c_str());
    ASSERT_EQ(289, p4.port);
    ASSERT_FALSE(butil::is_unix_sock_endpoint(p4));
    
    butil::EndPoint p5;
    ASSERT_EQ(-1, hostname2endpoint("localhost:-1", &p5));
    ASSERT_EQ(-1, hostname2endpoint("localhost:65536", &p5));
    ASSERT_EQ(0, hostname2endpoint("localhost:65535", &p5)) << berror();
    ASSERT_EQ(0, hostname2endpoint("localhost:0", &p5));
    ASSERT_FALSE(butil::is_unix_sock_endpoint(p5));

#ifdef BAIDU_INTERNAL
    butil::EndPoint p6;
    ASSERT_EQ(0, hostname2endpoint("tc-cm-et21.tc: 289 ", &p6));
    ASSERT_STREQ("10.23.249.73", butil::ip2str(p6.ip).c_str());
    ASSERT_EQ(289, p6.port);
#endif

    butil::EndPoint p7("/tmp/test.sock");
    ASSERT_EQ(butil::IP_ANY, p7.ip);
    ASSERT_EQ(0, p7.port);
    ASSERT_EQ("/tmp/test.sock", p7.socket_file);
    ASSERT_TRUE(butil::is_unix_sock_endpoint(p7));
}

TEST(EndPointTest, hash_table) {
    butil::hash_map<butil::EndPoint, int> m;
    butil::EndPoint ep1(butil::IP_ANY, 123);
    butil::EndPoint ep2(butil::IP_ANY, 456);
    ++m[ep1];
    ASSERT_TRUE(m.find(ep1) != m.end());
    ASSERT_EQ(1, m.find(ep1)->second);
    ASSERT_EQ(1u, m.size());

    ++m[ep1];
    ASSERT_TRUE(m.find(ep1) != m.end());
    ASSERT_EQ(2, m.find(ep1)->second);
    ASSERT_EQ(1u, m.size());

    ++m[ep2];
    ASSERT_TRUE(m.find(ep2) != m.end());
    ASSERT_EQ(1, m.find(ep2)->second);
    ASSERT_EQ(2u, m.size());
}

}
