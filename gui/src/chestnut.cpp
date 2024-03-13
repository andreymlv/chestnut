#include "chestnut.hpp"

void Chestnut::run_room(quint16 port) {
  if (rooms.contains(port)) {
    return;
  }
  rooms.emplace(port, new Room(port));

  QString request{
      "connect " + QString::number(socket.localAddress().toIPv4Address()) +
      " " + QString::number(port)
  };
  socket.write(request.toUtf8());
}

void Chestnut::stop_room(quint16 port) {
  if (!rooms.contains(port)) {
    return;
  }
  rooms.erase(port);

  QString request{
      "disconnect " + QString::number(socket.localAddress().toIPv4Address()) +
      " " + QString::number(port)
  };
  socket.write(request.toUtf8());
}
