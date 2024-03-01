#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "application.hpp"

int main(int argc, char** argv) {
  QGuiApplication app(argc, argv);

  /* All objects must be constructed before engine */
  ApplicationData data(&app);

  QQmlApplicationEngine engine;
  const QUrl url(u"qrc:/andreymlv/chestnut/Main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
      Qt::QueuedConnection);
  auto* context = engine.rootContext();
  context->setContextProperty("applicationData", &data);
  engine.load(url);

  return app.exec();
}
