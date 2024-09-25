#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QDebug>

#include "ControllerControl.h"

 int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QTimer timer;
    ControllerControl cc;
    QObject::connect(&cc, &ControllerControl::controlSignalSent, &timer, [&]() {
        timer.start(2000);
        qDebug() << "控制信号已发送 " ;
    });
    QObject::connect(&timer, &QTimer::timeout, &cc, [&]() {
        // cc.receiveControlResult("success");
        cc.receiveControlResult(QByteArray());
    });
    engine.rootContext()->setContextProperty("controllerControl", &cc);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();

}
