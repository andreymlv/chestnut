#pragma once

#include <QDateTime>
#include <QObject>

class ApplicationData : public QObject {
  Q_OBJECT
 public:
  explicit ApplicationData(QObject *parent) : QObject(parent) {}

  Q_INVOKABLE QDateTime getCurrentDateTime() const { return QDateTime::currentDateTime(); }
};