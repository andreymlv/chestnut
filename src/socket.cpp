#include "socket.hpp"

UdpSocket::UdpSocket(int handle) : handle(handle) {}

UdpSocket::~UdpSocket() { close(handle); }

std::optional<UdpSocket> UdpSocket::bind(std::unique_ptr<SocketAddr> addr) {
  auto ip = addr->ip()->to_string();
  auto port = std::to_string(addr->port());
  int rv;
  addrinfo *result = nullptr;
  addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
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

std::optional<std::pair<size_t, std::unique_ptr<SocketAddr>>>
UdpSocket::recv_from(std::span<std::byte> buf) const {
  sockaddr_storage their_addr{0};
  socklen_t addr_len = sizeof(their_addr);
  auto bytes = ::recvfrom(handle, buf.data(), buf.size_bytes(), 0,
                          (struct sockaddr *)&their_addr, &addr_len);
  if (bytes < 0) {
    fmt::println(stderr, "recvfrom: {}", strerror(errno));
    return {};
  }
  return std::make_pair(bytes, nullptr);
}

std::optional<size_t> UdpSocket::send_to(
    std::span<std::byte> buf, std::unique_ptr<SocketAddr> addr) const {
  auto bytes = sendto(handle, buf.data(), buf.size(), 0, nullptr, 0);
  if (bytes < 0) {
    fmt::println(stderr, "sendto: {}", strerror(errno));
    return {};
  }
  return bytes;
}
