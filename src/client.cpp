#include "client.hpp"

Client::Client(const M &m, QObject *parent) : QObject(parent), m(std::move(m)) {
  connect(m.pullTimer, &QTimer::timeout, this, &Client::record);
  connect(m.pushTimer, &QTimer::timeout, this, &Client::playback);
  m.pullTimer->start(10);
  m.pushTimer->start(10);
}

Client::~Client() {
  if (m.socket) {
    if (m.socket->isOpen()) {
      m.socket->close();
    }
    delete m.socket;
  }
  if (m.audioSource) {
    m.audioSource->stop();
    delete m.audioSource;
  }
  if (m.audioSink) {
    m.audioSink->stop();
    delete m.audioSink;
  }
  m.pullTimer->stop();
  m.pushTimer->stop();
}

Client Client::create(const SockAddr &addr, const QAudioDevice &deviceInfo, QObject *parent) {
  auto *socket = new QUdpSocket(parent);
  socket->bind();
  socket->connectToHost(addr.ip, addr.port);

  auto *audioInput = new QAudioInput(deviceInfo, parent);
  QAudioFormat formatAudio;
  formatAudio.setSampleRate(16000);
  formatAudio.setChannelCount(1);
  formatAudio.setSampleFormat(QAudioFormat::UInt8);
  auto *audioSource = new QAudioSource(deviceInfo, formatAudio, parent);
  audioSource->setBufferSize(4000);
  auto *audioSink = new QAudioSink(formatAudio, parent);

  auto *source = audioSource->start();
  auto *sink = audioSink->start();
  return Client(
      M{
          .addr = addr,
          .socket = socket,
          .audioSource = audioSource,
          .audioSink = audioSink,
          .source = source,
          .sink = sink,
          .pullTimer = new QTimer,
          .pushTimer = new QTimer,
      },
      parent);
}

void Client::record() {
  auto in = m.source->readAll();
  qDebug() << "sended" << m.socket->write(in);
}

void Client::playback() {
  auto bytes = m.audioSink->bytesFree();
  qDebug() << "can playback" << bytes;
  auto out = this->m.socket->readAll();
  this->m.sink->write(out);
}
