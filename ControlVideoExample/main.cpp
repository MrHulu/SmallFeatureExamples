#include <QGuiApplication>
#include <QQmlApplicationEngine>

 int main(int argc, char *argv[]) {

    qputenv("QT_MULTIMEDIA_PREFERRED_PLUGINS", "windowsmediafoundation");
    // qputenv("QT_MULTIMEDIA_PREFERRED_PLUGINS", "directshow"); //DirectShowPlayerService::doRender: Unresolved error code 0x8004022a (IDispatch error #42)
    // qputenv("QT_MULTIMEDIA_PREFERRED_PLUGINS", "gstreamer"); // DirectShowPlayerService::doRender: Unresolved error code 0x8004022a (IDispatch error #42)
    
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();

}
