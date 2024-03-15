#pragma once

#include <QRegularExpression>
#include <QTcpServer>
#include <QTcpSocket>

#include "room.hpp"
#include "sockaddr.hpp"

class Discover : public QObject {
  Q_OBJECT

 public:
  explicit Discover(quint16 port, QObject* parent = nullptr)
      : QObject(parent), server(this) {
    if (!server.listen(QHostAddress::AnyIPv4, port)) {
      qDebug() << server.errorString();
    }
    connect(&server, &QTcpServer::newConnection, this, &Discover::handle);
  }

 private slots:
  void handle() {
    auto* socket = server.nextPendingConnection();
    connect(
        socket, &QAbstractSocket::disconnected, socket, &QObject::deleteLater
    );
    auto buffer = QString::fromUtf8(socket->readAll())
                      .simplified()
                      .split(QRegularExpression("\\s+"));

    auto command = buffer[0];

    if (command.startsWith("list")) {
      // TODO: send all rooms

    } else if (command.startsWith("new")) {
      if (buffer.length() > 1) {
        // register room from other host
      } else {
        host_room();
      }
    } else if (command.startsWith("del")) {
    }

    socket->disconnectFromHost();
  }

 private:
  void host_room(quint16 port = 0) {
    auto room = std::make_unique<Room>(port);
    /* if (local_rooms.contains(room->addr().port)) { */
    /*   // TODO: try more times to host? */
    /*   return; */
    /* } */
    local_rooms.insert(std::move(room));
  }

  QStringList discovered() const {
    QStringList res;

    return res;
  }

  QStringList local() const {
    QStringList res;
    for (auto& room : local_rooms) {
      res << room->addr() << ",";
    }
    qDebug() << res;
    return res;
  }

  QTcpServer server;

  QSet<SockAddr> user_rooms;
  QSet<std::unique_ptr<Room>> local_rooms;
};
