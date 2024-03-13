#include "client.hpp"

AudioInfo::AudioInfo(QAudioFormat const& format) : m_format(format) {}

void AudioInfo::start() {
  open(QIODevice::WriteOnly);
}

void AudioInfo::stop() {
  close();
}

qint64 AudioInfo::readData(char* /* data */, qint64 /* maxlen */) {
  return 0;
}

qreal AudioInfo::calculateLevel(char const* data, qint64 len) const {
  int const channelBytes = m_format.bytesPerSample();
  int const sampleBytes = m_format.bytesPerFrame();
  int const numSamples = len / sampleBytes;

  float maxValue = 0;
  auto* ptr = reinterpret_cast<unsigned char const*>(data);

  for (int i = 0; i < numSamples; ++i) {
    for (int j = 0; j < m_format.channelCount(); ++j) {
      float value = m_format.normalizedSampleValue(ptr);

      maxValue = qMax(value, maxValue);
      ptr += channelBytes;
    }
  }
  return maxValue;
}

qint64 AudioInfo::writeData(char const* data, qint64 len) {
  m_level = calculateLevel(data, len);

  emit levelChanged(m_level);

  return len;
}

Client::Client(M const& m, QObject* parent) : QObject(parent), m(std::move(m)) {
  auto* io = m.audioInput->start();
  if (!io)
    return;
  connect(io, &QIODevice::readyRead, [this, &io, &m] {
    static qint64 const BufferSize = 4096;
    qint64 const len = qMin(m.audioInput->bytesAvailable(), BufferSize);
    qDebug() << len;

    QByteArray buffer(len, 0);
    qint64 l = io->read(buffer.data(), len);

    if (m.socket->write(buffer) < 0) {
      qDebug() << "Server is shutdown?";
      stop();
      return;
    }
  });

  /* connect(m.pullTimer, &QTimer::timeout, this, &Client::record); */
  connect(m.pushTimer, &QTimer::timeout, this, &Client::playback);
}

Client::~Client() {
  m.pushTimer->stop();
  if (m.socket) {
    if (m.socket->isOpen()) {
      m.socket->close();
    }
    delete m.socket;
  }
  if (m.audioSink) {
    m.audioSink->stop();
    delete m.audioSink;
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
  format.setSampleRate(48000);
  format.setChannelCount(1);
  format.setSampleFormat(QAudioFormat::Int16);
  auto* audioInfo = new AudioInfo(format);
  auto* audioInput = new QAudioSource(deviceInfo, format);
  auto* audioSink = new QAudioSink(format, parent);

  int opusError = OPUS_OK;
  auto* encoder = opus_encoder_create(
    16000,
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

  audioInfo->start();

  auto* sink = audioSink->start();
  return Client(
    M {
      .addr = addr,
      .socket = socket,
      .audioSink = audioSink,
      .sink = sink,
      .pushTimer = new QTimer,
      .audioInfo = audioInfo,
      .audioInput = audioInput,
      .encoder = encoder,
      .decoder = decoder,
    },
    parent
  );
}

void Client::record(QIODevice* io) {}

void Client::playback() {
  auto bytes = m.audioSink->bytesFree();
  if (!bytes) {
    return;
  }
  auto out = this->m.socket->readAll();
  this->m.sink->write(out);
}

void Client::start() {
  /* m.pullTimer->start(); */
  m.pushTimer->start();
}

void Client::stop() {
  /* m.pullTimer->stop(); */
  m.pushTimer->stop();
  m.audioSink->stop();
  /* m.audioSource->stop(); */
}
