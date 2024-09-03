#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlPropertyMap>
#include <QQmlContext>
#include <QDebug>
#include <QTimer>

#include "Parameter.h"

 int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    //用法一： QQmlPropertyMap 的值为基本类型
    QQmlPropertyMap basePropertyMap;
    basePropertyMap.insert("boolValue", true);
    basePropertyMap.insert("intValue", 123);
    basePropertyMap.insert("StringValue", QString("Hello World"));
    // QML to C++
    QObject::connect(&basePropertyMap, &QQmlPropertyMap::valueChanged, [](const QString &key, const QVariant &value) {
        qDebug() << "basePropertyMap:" <<key << value;
    });
    // C++ to QML
    QTimer timer; timer.start(1000);
    QObject::connect(&timer, &QTimer::timeout, [&basePropertyMap](){
        basePropertyMap.insert("boolValue", !basePropertyMap.value("boolValue").toBool());
        basePropertyMap.insert("intValue", basePropertyMap.value("intValue").toInt() + 1);
    });
    engine.rootContext()->setContextProperty("basePropertyMap", &basePropertyMap);

    //用法二： QQmlPropertyMap 的值为对象
    QQmlPropertyMap objectPropertyMap;
    auto p1 = new Parameter();
    auto p2 = new Parameter();
    objectPropertyMap.insert("parameter1", QVariant::fromValue(p1));
    objectPropertyMap.insert("parameter2", QVariant::fromValue(p2));
    // QML to C++
    QObject::connect(p1, &Parameter::valueChanged, [&]() {
        // qDebug() << "parameter1 value changed:" << p1->value();
    });
    QObject::connect(p1, &Parameter::enabledChanged, [&]() {
        qDebug() << "parameter1 enabled changed:" << p1->enabled();
    });
    QObject::connect(p2, &Parameter::valueChanged, [&]() {
        // qDebug() << "parameter2 value changed:" << p2->value();
    });
    QObject::connect(p2, &Parameter::enabledChanged, [&]() {
        qDebug() << "parameter2 enabled changed:" << p2->enabled();
    });
    // C++ to QML
    QTimer timer2; timer2.start(1000);
    QObject::connect(&timer2, &QTimer::timeout, [&objectPropertyMap](){
        auto p1 = objectPropertyMap.value("parameter1").value<Parameter*>();
        auto p2 = objectPropertyMap.value("parameter2").value<Parameter*>();
        p1->setValue(p1->value().toInt() + 1);
        p2->setValue(p2->value().toInt() - 1);
    });
    engine.rootContext()->setContextProperty("objectPropertyMap", &objectPropertyMap);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();

}
