#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

/* #include "application.hpp" */

int main(int argc, char** argv) {
  QGuiApplication app(argc, argv);

  QGuiApplication::setWindowIcon(QIcon(":/icon.png"));

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
      Qt::QueuedConnection);
  engine.loadFromModule("chestnut", "Main");

  return app.exec();
}
