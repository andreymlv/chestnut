#pragma once

#include <QAudioInput>
#include <QAudioSink>
#include <QAudioSource>
#include <QNetworkDatagram>
#include <QTimer>
#include <QUdpSocket>

#include "sockaddr.hpp"

class Client : public QObject {
  Q_OBJECT

  struct M {
    SockAddr addr = SockAddr{QHostAddress::LocalHost, 0};
    QUdpSocket *socket = nullptr;
    QAudioSource *audioSource = nullptr;
    QAudioSink *audioSink = nullptr;
    QIODevice *source = nullptr;
    QIODevice *sink = nullptr;
    QTimer *pullTimer = nullptr;
    QTimer *pushTimer = nullptr;
  } m;

  Client(const M &m, QObject *parent);

 public:
  Client(Client &&other) : m(std::move(other.m)) {}

  Client &operator=(Client &&other) {
    m = std::move(other.m);
    return *this;
  }

  ~Client();

  static Client create(const SockAddr &addr, const QAudioDevice &deviceInfo, QObject *parent);

  void record();
  void playback();
};
