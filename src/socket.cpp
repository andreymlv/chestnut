#include "socket.hpp"

#include <memory>

#include "fmt/core.h"

UdpSocket::UdpSocket(int handle) : handle(handle) {}

UdpSocket::UdpSocket() : UdpSocket(-1) {}

UdpSocket::~UdpSocket() {
  if (handle != -1) {
    close(handle);
  }
}

UdpSocket &UdpSocket::operator=(UdpSocket &&other) {
  handle = std::exchange(other.handle, -1);
  return *this;
}

UdpSocket::UdpSocket(UdpSocket &&other) : handle(std::exchange(other.handle, -1)) {}

std::optional<UdpSocket> UdpSocket::bind(std::unique_ptr<SocketAddr> addr) {
  auto ip = addr->ip()->to_string();
  auto port = std::to_string(addr->port());
  int rv;
  addrinfo *result = nullptr;
  addrinfo hints{0};
  hints.ai_family = AF_INET;      /* IPv4 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  if ((rv = getaddrinfo(ip.data(), port.data(), &hints, &result)) != 0) {
    fmt::println(stderr, "getaddrinfo: {}", gai_strerror(rv));
    return {};
  }

  int handle = -1;
  addrinfo *p = nullptr;
  for (p = result; p != nullptr; p = p->ai_next) {
    if ((handle = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      fmt::println(stderr, "socket: {}", strerror(errno));
      continue;
    }

    if (::bind(handle, p->ai_addr, p->ai_addrlen) == -1) {
      close(handle);
      fmt::println(stderr, "bind: {}", strerror(errno));
      continue;
    }

    break;
  }

  freeaddrinfo(result);

  if (!p) {
    fmt::println(stderr, "failed to bind socket");
    return {};
  }

  if (!reuse_socket(handle)) {
    return {};
  }

  return UdpSocket(handle);
}

std::optional<std::pair<size_t, std::unique_ptr<SocketAddr>>> UdpSocket::recv_from(std::span<std::byte> buf) const {
  sockaddr_in addr{0};
  socklen_t addr_len = sizeof(addr);
  auto bytes = recvfrom(handle, buf.data(), buf.size_bytes(), 0, reinterpret_cast<sockaddr *>(&addr), &addr_len);
  if (bytes < 0) {
    fmt::println(stderr, "recvfrom: {}", strerror(errno));
    return {};
  }
  auto sock_addr = std::make_unique<SocketAddrV4>(SocketAddrV4::from(addr));
  return std::make_pair(bytes, std::move(sock_addr));
}

std::optional<size_t> UdpSocket::send_to(std::span<std::byte> buf, std::unique_ptr<SocketAddr> addr) const {
  sockaddr_in their_addr{0};
  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(addr->port());
  inet_pton(AF_INET, addr->ip()->to_string().data(), &their_addr.sin_addr);
  socklen_t addr_len = sizeof(their_addr);
  auto bytes = sendto(handle, buf.data(), buf.size_bytes(), 0, reinterpret_cast<sockaddr *>(&their_addr), addr_len);
  if (bytes < 0) {
    fmt::println(stderr, "sendto: {}", strerror(errno));
    return {};
  }
  return bytes;
}
