#pragma once

#include <opus.h>
#include <qmediadevices.h>

#include <QAudioInput>
#include <QAudioSink>
#include <QAudioSource>
#include <QNetworkDatagram>
#include <QTimer>
#include <QUdpSocket>

#include "sockaddr.hpp"

class Client: public QObject {
  Q_OBJECT

  struct M {
    SockAddr addr = SockAddr {QHostAddress::LocalHost, 0};
    QUdpSocket* socket = nullptr;

    QAudioSink* audioSink = nullptr;
    QIODevice* sink = nullptr;
    QTimer* pushTimer = nullptr;
    OpusDecoder* decoder = nullptr;

    QAudioSource* audioInput = nullptr;
    QIODevice* source = nullptr;
    QTimer* pullTimer = nullptr;
    OpusEncoder* encoder = nullptr;
  } m;

  Client(M const& m, QObject* parent);

 public:
  Client(Client&& other) : m(std::move(other.m)) {}

  Client& operator=(Client&& other) {
    m = std::move(other.m);
    return *this;
  }

  ~Client();

  void start();

  void stop();

  static Client
  create(SockAddr const& addr, QAudioDevice const& deviceInfo, QObject* parent);

 private slots:
  void record();
  void playback();
};
