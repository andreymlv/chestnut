#pragma once

#include <QAudioDevice>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMediaDevices>
#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QTcpSocket>
#include <unordered_map>

#include "client.hpp"
#include "room.hpp"

class Chestnut : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(QStringList rooms MEMBER rooms_model NOTIFY rooms_changed)
 public:
  explicit Chestnut(QObject* parent = nullptr)
      : QObject(parent),
        input_device(QMediaDevices::defaultAudioInput()),
        settings("chestnut", "chestnut"),
        socket(this),
        discover_timer(this) {
    if (input_device.isNull()) {
      qDebug() << "There is no audio input device available.";
    }

    // TODO: implement read from settings
    settings.setValue("discover/addr", "127.0.0.1");
    settings.setValue("discover/port", 5252);

    auto addr = settings.value("discover/addr", "127.0.0.1").toString();
    auto port = settings.value("discover/port", 5252).toInt();

    socket.bind();
    socket.connectToHost(addr, port);

    connect(&discover_timer, &QTimer::timeout, this, &Chestnut::discover);
    discover_timer.start(1000);
  }

  Q_INVOKABLE void run_room(quint16 port);

  Q_INVOKABLE void run_room(QString const& port) { run_room(port.toUShort()); }

  Q_INVOKABLE void stop_room(quint16 port);

  Q_INVOKABLE void stop_room(QString const& port) {
    stop_room(port.toUShort());
  }

  Q_INVOKABLE void client_connect(QString const& addressWithPort) {
    if (connected) {
      return;
    }

    auto address = addressWithPort.split(":");
    if (address.size() != 2) {
      return;
    }
    SockAddr addr(QHostAddress(address[0]), address[1].toUShort());

    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    client = std::make_unique<Client>(
        std::move(addr), input_device, std::move(format), this
    );
    client->start();
    connected = true;
  }

  Q_INVOKABLE void client_disconnect() {
    if (!connected) {
      return;
    }
    client.reset();
    connected = false;
  }

 signals:
  void rooms_changed();

 private slots:
  void discover() {
    if (socket.write("discover") < 0) {
      qDebug() << "Discover server is down?";
    }
    QString rooms(socket.readAll());
    rooms_model = rooms.split(",");
    emit rooms_changed();
  }

 private:
  std::unordered_map<quint16, std::unique_ptr<Room>> rooms;
  QStringList rooms_model;

  std::unique_ptr<Client> client;
  QAudioDevice input_device;
  bool connected = false;

  QSettings settings;

  QTcpSocket socket;
  QTimer discover_timer;
};
