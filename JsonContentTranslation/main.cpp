// main.cpp
#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QTranslator>
#include <QDebug>
#include "src/Test.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // 实例化Test对象
    Test test;
    engine.rootContext()->setContextProperty("test", &test);
    

    // 加载翻译文件
    QMap<QString, QTranslator*> translators;
    auto path = QString("%1/translations").arg(QCoreApplication::applicationDirPath());
    QDir dir(path);
    for (const auto& info : dir.entryInfoList({"*.qm"}, QDir::Files, QDir::Size)) {
        if (auto translator = new QTranslator(qApp);
            translator->load(info.absoluteFilePath())) {
                translators.insert(translator->language(), translator);
            }
    }
    // 中文是源语言，所以需要插入一个空翻译器
    translators.insert("zh_CN", nullptr);

    // 设置翻译语言
    auto currentLanguage = test.isChinese() ? "zh_CN" : "en_US";
    engine.setUiLanguage(currentLanguage);
    qApp->installTranslator(translators[currentLanguage]);
    QObject::connect(&test, &Test::isChineseChanged, &engine, [&]() {
        // 先清除所有现有翻译器
        const auto translator = translators[currentLanguage];
        qApp->removeTranslator(translator);
        
        // 重新加载翻译文件;
        currentLanguage = test.isChinese() ? "zh_CN" : "en_US";
        qApp->installTranslator(translators[currentLanguage]);
        
        // 设置新的UI语言
        engine.setUiLanguage(currentLanguage);
        qDebug() << "更新翻译：" << (test.isChinese() ? "zh_CN" : "en_US");
    });
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
