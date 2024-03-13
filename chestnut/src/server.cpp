#include "server.hpp"

#include <QNetworkDatagram>
#include <QString>

#include "sockaddr.hpp"

Server::Server(quint16 port, QObject* parent) :
  QObject(parent),
  m(new ServerImpl(port)) {
  if (!m->accept()) {
    qDebug() << "Failed to bind to specified address";
    return;
  }
  qDebug() << "Server started on port" << m->socket->localPort();
  connect(m->socket.get(), &QUdpSocket::readyRead, this, &Server::handle);
}

void Server::handle() {
  while (m->socket->hasPendingDatagrams()) {
    QByteArray datagramBuffer;
    datagramBuffer.resize(m->socket->pendingDatagramSize());

    QHostAddress addr;
    quint16 port;
    m->socket->readDatagram(
      datagramBuffer.data(),
      datagramBuffer.size(),
      &addr,
      &port
    );

    const SockAddr currentClient(addr, port);
    if (!m->clients.contains(currentClient)) {
      qInfo().noquote() << tr("%1:%2 has joined")
                             .arg(currentClient.ip.toString())
                             .arg(currentClient.port);
      m->clients.insert(currentClient);
    }

    m->broadcast(datagramBuffer.data(), datagramBuffer.length(), currentClient);
  }
}
