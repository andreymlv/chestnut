
#include <QAudioDevice>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMediaDevices>
#include <client.hpp>
#include <room.hpp>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("chestnut is a VOIP applicaition");
  parser.addHelpOption();

  QCommandLineOption clAddress(
      QStringList() << "a"
                    << "address",
      QObject::tr("Room adress."),
      "address"
  );
  parser.addOption(clAddress);

  QCommandLineOption clPort(
      QStringList() << "p"
                    << "port",
      QObject::tr("Room port."),
      "port"
  );
  parser.addOption(clPort);

  parser.process(app);

  auto hostMode = parser.isSet(clPort) && !parser.isSet(clAddress);
  auto clientMode = parser.isSet(clPort) && parser.isSet(clAddress);

  if (hostMode) {
    auto port = parser.value(clPort).toUShort();
    Room room(port, &app);
    return QCoreApplication::exec();
  }

  if (clientMode) {
    QAudioDevice const inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
      qDebug() << "There is no audio input device available.";
      return 1;
    }
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);
    QHostAddress ip(parser.value(clAddress));
    auto port = parser.value(clPort).toUShort();
    SockAddr addr(ip, port);
    Client client{
        std::move(addr), std::move(inputDevice), std::move(format), &app
    };
    client.start();
    return QCoreApplication::exec();
  }

  parser.showHelp();
}
