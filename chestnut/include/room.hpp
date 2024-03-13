#pragma once

#include <QDateTime>
#include <QScopedPointer>
#include <QSet>
#include <QUdpSocket>

#include "sockaddr.hpp"

class Room final : public QObject {
  Q_OBJECT

 public:
  explicit Room(quint16 port, QObject* parent = nullptr);
  ~Room() { qDebug() << "Room on port" << port << "closing"; }

 private slots:
  void handle();

 private:
  bool accept() noexcept {
    return socket.bind(
        QHostAddress::AnyIPv4,
        port,
        QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint
    );
  }

  void broadcast(const char* msg, int len, const SockAddr& src) noexcept {
    for (auto it = clients.begin(); it != clients.end(); ++it) {
      if (*it == src) continue;  // Skip the original sender
      socket.writeDatagram(msg, len, it->ip, it->port);
    }
  }

  quint16 port;
  QUdpSocket socket;
  QSet<SockAddr> clients;
};
