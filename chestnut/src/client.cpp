#include "client.hpp"

Client::Client(
    SockAddr const& addr,
    QAudioDevice const& deviceInfo,
    QAudioFormat const& format,
    QObject* parent
)
    : QObject(parent),
      socket(this),
      pullTimer(this),
      pushTimer(this),
      encoder(
          opus_encoder_create(
              format.sampleRate(),
              format.channelCount(),
              OPUS_APPLICATION_VOIP,
              nullptr
          ),
          opus_encoder_destroy
      ),
      decoder(
          opus_decoder_create(
              format.sampleRate(), format.channelCount(), nullptr
          ),
          opus_decoder_destroy
      ) {
  socket.bind();
  socket.connectToHost(addr.ip, addr.port);

  audioInput = std::make_unique<QAudioSource>(deviceInfo, format);
  audioInput->setBufferSize(4096);
  audioSink = std::make_unique<QAudioSink>(format, parent);

  source = audioInput->start();
  sink = audioSink->start();

  connect(&pullTimer, &QTimer::timeout, this, &Client::record);
  connect(&pushTimer, &QTimer::timeout, this, &Client::playback);
}

Client::~Client() {
  stop();
  if (socket.isOpen()) {
    socket.close();
  }
}

void Client::record() {
  static qint64 const BufferSize = 4096;
  qint64 const len = qMin(audioInput->bytesAvailable(), BufferSize);

  QByteArray buffer(len, 0);
  qint64 l = source->read(buffer.data(), len);

  if (socket.write(buffer) < 0) {
    qDebug() << "Server is shutdown?";
    stop();
    return;
  }
}

void Client::playback() {
  auto bytes = audioSink->bytesFree();
  auto out = socket.read(bytes);
  sink->write(out);
}

void Client::start() {
  pullTimer.start(10);
  pushTimer.start(10);
}

void Client::stop() {
  pullTimer.stop();
  pushTimer.stop();
  audioSink->stop();
  audioInput->stop();
}
