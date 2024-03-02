#pragma once

#include <QDateTime>
#include <QObject>
#include <QQmlEngine>

class Chestnut : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
 public:
  explicit Chestnut(QObject *parent = nullptr) : QObject(parent) {}

  Q_INVOKABLE QDateTime getCurrentDateTime() const { return QDateTime::currentDateTime(); }
};
