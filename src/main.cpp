
#include <QAudioDevice>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMediaDevices>

#include "client.hpp"
#include "server.hpp"

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);
  auto args = app.arguments();
  if (args.length() == 2) {
    quint16 port(args[1].toUShort());
    Server server(port, &app);
    return QCoreApplication::exec();
  } else if (args.length() == 3) {
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
      qDebug() << "There is no audio input device available.";
      return 1;
    }
    QHostAddress ip(args[1]);
    quint16 port(args[2].toUShort());
    SockAddr addr(ip, port);
    Client client = Client::create(std::move(addr), std::move(inputDevice), &app);
    client.record();

    return QCoreApplication::exec();
  }
  qDebug() << "Usage [client mode]: chestnut <ip address> <port>";
  qDebug() << "Usage [server mode]: chestnut <port>";
  return 2;
}
