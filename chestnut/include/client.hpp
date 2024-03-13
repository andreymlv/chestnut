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

class AudioInfo: public QIODevice {
  Q_OBJECT

 public:
  AudioInfo(QAudioFormat const& format);

  void start();
  void stop();

  qreal level() const {
    return m_level;
  }

  qint64 readData(char* data, qint64 maxlen) override;
  qint64 writeData(char const* data, qint64 len) override;

  qreal calculateLevel(char const* data, qint64 len) const;

 signals:
  void levelChanged(qreal level);

 private:
  QAudioFormat const m_format;
  qreal m_level = 0.0;  // 0.0 <= m_level <= 1.0
};

class Client: public QObject {
  Q_OBJECT

  struct M {
    SockAddr addr = SockAddr {QHostAddress::LocalHost, 0};
    QUdpSocket* socket = nullptr;
    QAudioSink* audioSink = nullptr;
    QIODevice* sink = nullptr;
    QTimer* pushTimer = nullptr;

    QMediaDevices* m_devices = nullptr;
    AudioInfo* audioInfo = nullptr;
    QAudioSource* audioInput = nullptr;

    OpusEncoder* encoder = nullptr;
    OpusDecoder* decoder = nullptr;
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
  void record(QIODevice* io);
  void playback();
};
