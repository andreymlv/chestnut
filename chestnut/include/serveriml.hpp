#pragma once

#include <QUdpSocket>

#include "sockaddr.hpp"

struct ServerImpl {
  quint16 port;
  QScopedPointer<QUdpSocket> socket;
  QSet<SockAddr> clients;

  explicit ServerImpl(quint16 port) noexcept :
    port(port),
    socket(new QUdpSocket) {}

  /**
   * @brief Handles new connections from remote hosts.
   * @return True when successful, false otherwise.
   */
  bool accept() noexcept {
    assert(!socket.isNull());
    return socket->bind(
      QHostAddress::AnyIPv4,
      port,
      QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint
    );
  }

  /**
   * @brief Broadcasts a message to all connected clients excluding the originator.
   * @param[in] msg Message buffer containing the payload to send.
   * @param[in] len Length of the message buffer.
   * @param[in] src Address of the source host sending the message.
   */
  void broadcast(const char* msg, int len, const SockAddr& src) noexcept {
    /* assert(!msg || len <= 0); */
    for (auto it = clients.begin(); it != clients.end(); ++it) {
      if (*it == src)
        continue;  // Skip the original sender
      socket->writeDatagram(msg, len, it->ip, it->port);
    }
  }
};
