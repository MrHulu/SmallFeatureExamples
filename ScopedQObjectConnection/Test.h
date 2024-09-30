#pragma once 

#include "ScopedConnection.h"

#include <QObject>
#include <QDebug>
#include <QTimer>

class A {
public:
    A() { 
        t = new QTimer; 
        random = qrand();
        t->start(1000);
    }
    ~A() {  if(t) t->deleteLater(); }
    QTimer* t;
    int random;
};

class Test : public QObject {
    Q_OBJECT
public:
    void setA(A* newA) {
        a = newA;
        if (a != nullptr && a->t != nullptr) {
            buttonConnection = QObject::connect(a->t, &QTimer::timeout, this, &Test::onSlots);
        }else{
            buttonConnection = QMetaObject::Connection();
        }
    }

public slots:
    void onSlots() {
        if(a)
            qDebug() << "Timeout" << a->random;
        else
            qDebug() << "Timeout" << "a is nullptr";
    }

private:
    A* a = nullptr;  // 当前的 A 对象
    ScopedConnection buttonConnection;  // 自动管理的信号连接
};
