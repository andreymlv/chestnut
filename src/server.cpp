#include "server.hpp"

#include <QNetworkDatagram>
#include <utility>

Server::Server(quint16 port, QObject* parent)
    : QObject(parent),
      m(M{
          .socket = QUdpSocket(this),
      }) {
  m.socket.bind(QHostAddress::AnyIPv4, port);
  connect(&m.socket, &QUdpSocket::readyRead, this, &Server::run);
}

Server::~Server() {
  if (m.socket.isOpen()) m.socket.close();
}

void Server::run() {
  while (m.socket.hasPendingDatagrams()) {
    const auto datagram = m.socket.receiveDatagram();
    const auto buf = datagram.data();
    const auto sender = SockAddr(datagram.senderAddress(), datagram.senderPort());
    if (!m.clients.contains(sender)) m.clients.insert(sender);
    for (const auto& client : std::as_const(m.clients)) {
      if (client != sender) {
        auto _ = m.socket.writeDatagram(buf, client.ip, client.port);
      }
    }
  }
}
