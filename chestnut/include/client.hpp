#pragma once

#include <opus.h>
#include <qmediadevices.h>

#include <QAudioInput>
#include <QAudioSink>
#include <QAudioSource>
#include <QNetworkDatagram>
#include <QTimer>
#include <QUdpSocket>
#include <memory>

#include "sockaddr.hpp"

class Client : public QObject {
  Q_OBJECT

  QUdpSocket socket;

  QAudioFormat format;

  std::unique_ptr<QAudioSink> audioSink;
  QIODevice* sink = nullptr;
  QTimer pushTimer;
  std::unique_ptr<OpusDecoder, std::function<void(OpusDecoder*)>> decoder;

  std::unique_ptr<QAudioSource> audioInput;
  QIODevice* source = nullptr;
  QTimer pullTimer;
  std::unique_ptr<OpusEncoder, std::function<void(OpusEncoder*)>> encoder;

 public:
  Client(
      SockAddr const& addr,
      QAudioDevice const& deviceInfo,
      QAudioFormat const& format,
      QObject* parent = nullptr
  );
  ~Client();

  void start();

  void stop();

 private slots:
  void record();
  void playback();
};
