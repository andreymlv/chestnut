#include "room.hpp"

#include <QNetworkDatagram>
#include <QString>

#include "sockaddr.hpp"

Room::Room(quint16 port, QObject* parent)
    : QObject(parent), port(port), socket(this) {
  if (!accept()) {
    qDebug() << "Failed to bind to specified address";
    return;
  }
  qDebug() << "Room started on port" << socket.localPort();
  connect(&socket, &QUdpSocket::readyRead, this, &Room::handle);
}

void Room::handle() {
  while (socket.hasPendingDatagrams()) {
    QByteArray datagramBuffer;
    datagramBuffer.resize(socket.pendingDatagramSize());

    QHostAddress addr;
    quint16 port;
    socket.readDatagram(
        datagramBuffer.data(), datagramBuffer.size(), &addr, &port
    );

    const SockAddr currentClient(addr, port);
    if (!clients.contains(currentClient)) {
      qInfo().noquote() << tr("%1:%2 has joined")
                               .arg(currentClient.ip.toString())
                               .arg(currentClient.port);
      clients.insert(currentClient);
    }

    broadcast(datagramBuffer.data(), datagramBuffer.length(), currentClient);
  }
}
