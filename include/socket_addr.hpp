#pragma once

#include <memory>

#include "ip_addr.hpp"

class SocketAddr {
 public:
  virtual ~SocketAddr() = default;
  virtual const uint16_t port() const = 0;
  virtual const std::unique_ptr<IpAddr> ip() const = 0;
};

class SocketAddrV4 : public SocketAddr {
  struct {
    Ipv4Addr ip;
    uint16_t port;
  } m;

 public:
  SocketAddrV4(Ipv4Addr ip, uint16_t port) : m(ip, port) {}

  const uint16_t port() const override { return m.port; }

  const std::unique_ptr<IpAddr> ip() const override {
    auto [a, b, c, d] = m.ip.octets();
    return std::make_unique<Ipv4Addr>(Ipv4Addr::create(a, b, c, d));
  };
};

class SocketAddrV6 : public SocketAddr {
  struct {
    Ipv6Addr ip;
    uint16_t port;
    uint32_t flowinfo;
    uint32_t scope_id;
  } m;

 public:
  SocketAddrV6(Ipv6Addr ip, uint16_t port, uint32_t flowinfo, uint32_t scope_id)
      : m(ip, port, flowinfo, scope_id) {}

  const uint16_t port() const override { return m.port; }

  // TODO: implement this
  const std::unique_ptr<IpAddr> ip() const override {
    return std::make_unique<Ipv6Addr>(Ipv6Addr::create(0, 0, 0, 0, 0, 0, 0, 0));
  };
};
