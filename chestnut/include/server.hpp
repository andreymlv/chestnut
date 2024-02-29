#pragma once

#include <QSet>
#include <QUdpSocket>

#include "sockaddr.hpp"

class Server : public QObject {
  Q_OBJECT

  struct M {
    QUdpSocket socket;
    QSet<SockAddr> clients;
  } m;

  void run();

 public:
  Server(quint16 port, QObject *parent);
  Server(Server &&) = delete;
  Server(const Server &) = delete;
  Server &operator=(Server &&) = delete;
  Server &operator=(const Server &) = delete;
  ~Server();
};
