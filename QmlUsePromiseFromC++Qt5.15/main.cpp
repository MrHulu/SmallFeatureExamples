#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <QDebug>

#include "Test.h"
#include "ControllerControl.h"


int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QTimer timer;
    timer.setSingleShot(true);
    ControllerControl cc;
    Test t(&cc);
    QObject::connect(&cc, &ControllerControl::requestControl, &timer, [&](auto data) {
        timer.start(2000);
        qDebug() << "控制信号已发送 " ;
    });
    bool lastResult = false;
    QObject::connect(&timer, &QTimer::timeout, &cc, [&]() {
        // cc.receiveControlResult("success");
        lastResult = !lastResult;
        qDebug() << "本次操作结果： " << lastResult;
        cc.receiveControlResult(lastResult);
    });
    qmlRegisterType<ControllerControl>("ControllerControl", 1, 0, "ControllerControl");
    qmlRegisterType<Test>("ControllerControl", 1, 0, "Test");
    engine.rootContext()->setContextProperty("controllerControl", &cc);
    engine.rootContext()->setContextProperty("test", &t);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();

}
