
#include <QAudioDevice>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMediaDevices>
#include <client.hpp>
#include <server.hpp>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("chestnut is a VOIP applicaition");
  parser.addHelpOption();

  QCommandLineOption clAddress(QStringList() << "a"
                                             << "address",
                               QObject::tr("Server adress."));
  parser.addOption(clAddress);

  QCommandLineOption clPort(QStringList() << "p"
                                          << "port",
                            QObject::tr("Server port."));
  parser.addOption(clPort);

  parser.process(app);

  auto hostMode = parser.isSet(clPort) && !parser.isSet(clAddress);
  auto clientMode = parser.isSet(clPort) && parser.isSet(clAddress);

  if (hostMode) {
    auto port = parser.value(clPort).toUShort();
    Server server(port, &app);
    return QCoreApplication::exec();
  }

  if (clientMode) {
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
      qDebug() << "There is no audio input device available.";
      return 1;
    }
    QHostAddress ip(parser.value(clAddress));
    auto port = parser.value(clPort).toUShort();
    SockAddr addr(ip, port);
    Client client = Client::create(std::move(addr), std::move(inputDevice), &app);
    client.record();

    return QCoreApplication::exec();
  }

  parser.showHelp();
}
