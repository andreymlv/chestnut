#include "ip_addr.hpp"

const Ipv4Addr Ipv4Addr::LOCALHOST = Ipv4Addr::create(127, 0, 0, 1);
const Ipv4Addr Ipv4Addr::UNSPECIFIED = Ipv4Addr::create(0, 0, 0, 0);
const Ipv4Addr Ipv4Addr::BROADCAST = Ipv4Addr::create(255, 255, 255, 255);
