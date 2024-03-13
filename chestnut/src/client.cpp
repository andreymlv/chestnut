#include "client.hpp"

Client::Client(M const& m, QObject* parent) : QObject(parent), m(std::move(m)) {
  connect(m.pullTimer, &QTimer::timeout, this, &Client::record);
  connect(m.pushTimer, &QTimer::timeout, this, &Client::playback);
}

Client::~Client() {
  stop();
  if (m.socket) {
    if (m.socket->isOpen()) {
      m.socket->close();
    }
    delete m.socket;
  }
  opus_encoder_destroy(m.encoder);
  opus_decoder_destroy(m.decoder);
}

Client Client::create(
  SockAddr const& addr,
  QAudioDevice const& deviceInfo,
  QObject* parent
) {
  auto* socket = new QUdpSocket(parent);
  socket->bind();
  socket->connectToHost(addr.ip, addr.port);

  QAudioFormat format;
  format.setSampleRate(16000);
  format.setChannelCount(1);
  format.setSampleFormat(QAudioFormat::Int16);

  auto* audioInput = new QAudioSource(deviceInfo, format);
  audioInput->setBufferSize(4096);
  auto* audioSink = new QAudioSink(format, parent);

  int opusError = OPUS_OK;
  auto* encoder = opus_encoder_create(
    format.sampleRate(),
    format.channelCount(),
    OPUS_APPLICATION_VOIP,
    &opusError
  );
  if (opusError != OPUS_OK) {
    qWarning() << "Failed to create Opus encoder.";
  }

  auto* decoder = opus_decoder_create(16000, format.channelCount(), &opusError);
  if (opusError != OPUS_OK) {
    qWarning() << "Failed to create Opus encoder.";
  }

  auto* source = audioInput->start();
  auto* sink = audioSink->start();
  return Client(
    M {
      .addr = addr,
      .socket = socket,

      .audioSink = audioSink,
      .sink = sink,
      .pushTimer = new QTimer,
      .decoder = decoder,

      .audioInput = audioInput,
      .source = source,
      .pullTimer = new QTimer,
      .encoder = encoder,

    },
    parent
  );
}

void Client::record() {
  static qint64 const BufferSize = 4096;
  qint64 const len = qMin(m.audioInput->bytesAvailable(), BufferSize);

  QByteArray buffer(len, 0);
  qint64 l = m.source->read(buffer.data(), len);

  if (m.socket->write(buffer) < 0) {
    qDebug() << "Server is shutdown?";
    stop();
    return;
  }
}

void Client::playback() {
  auto bytes = m.audioSink->bytesFree();
  if (!bytes) {
    return;
  }
  auto out = this->m.socket->readAll();
  this->m.sink->write(out);
}

void Client::start() {
  m.pullTimer->start();
  m.pushTimer->start();
}

void Client::stop() {
  m.pullTimer->stop();
  m.pushTimer->stop();
  m.audioSink->stop();
  m.audioInput->stop();
}
