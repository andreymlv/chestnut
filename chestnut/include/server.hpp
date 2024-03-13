#pragma once

#include <QDateTime>
#include <QScopedPointer>
#include <QSet>
#include <QUdpSocket>
#include <memory>

#include "serveriml.hpp"

class Server final : public QObject {
  Q_OBJECT

 public:
  explicit Server(quint16 port, QObject *parent = nullptr);

 private slots:
  /**
   * @brief Processes incoming messages from all connected clients.
   */
  void handle();

 private:
  std::unique_ptr<ServerImpl> m;
};
