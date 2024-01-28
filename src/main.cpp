
#include <QCommandLineParser>
#include <QCoreApplication>

#include "server.hpp"

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);
  auto args = app.arguments();
  if (args.length() == 2) {
    quint16 port(args[1].toUShort());
    Server server(port, &app);
    return QCoreApplication::exec();
  } else if (args.length() == 3) {
    QHostAddress ip(args[1]);
    quint16 port(args[2].toUShort());
    SockAddr addr(ip, port);
    return QCoreApplication::exec();
  } else {
    return 1;
  }
  return 2;
}
