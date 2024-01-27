#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <utility>

#include "socket_addr.hpp"

#ifndef _WIN32
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif  // _WIN32

#include <fmt/core.h>
#include <fmt/ranges.h>

namespace {

inline bool reuse_socket(int handle) {
  int yes = 1;
  if (setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    fmt::println(stderr, "setsockopt: {}", strerror(errno));
    return false;
  }
  return true;
}

inline void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

}  // namespace

class UdpSocket {
  int handle;
  explicit UdpSocket(int handle);
  UdpSocket();

 public:
  UdpSocket(UdpSocket &&other);

  UdpSocket &operator=(UdpSocket &&other);

  ~UdpSocket();

  static std::optional<UdpSocket> bind(std::unique_ptr<SocketAddr> addr);

  std::optional<std::pair<size_t, std::unique_ptr<SocketAddr>>> recv_from(
      std::span<std::byte> buf) const;

  std::optional<size_t> send_to(std::span<std::byte> buf,
                                std::unique_ptr<SocketAddr> addr) const;
};
