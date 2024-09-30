#include "ScopedConnection.h"
#include "Test.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Test t;
    A* a = new A();
    t.setA(a);
    // 此时应该有信号连接
    
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &t, [&]() {
        qDebug() << "赋值新的 A 对象";
        A* b = new A();
        // 此时应该有信号连接，但是 a 对象的信号连接应该被断开
        t.setA(b);
    });
    timer.start(3000);


    return app.exec();
}
