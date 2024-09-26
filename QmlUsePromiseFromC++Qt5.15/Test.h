#include <QFutureWatcher>
#include <QFuture>
#include <QObject>

#include "ControllerControl.h"

class Test : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool result READ result NOTIFY resultChanged)
public:
    Test() = default;
    Test(ControllerControl *cc, QObject *parent = nullptr) 
    : QObject(parent), m_cc(cc), m_futureWatcher(new QFutureWatcher<bool>(this))
    {  
        connect(m_futureWatcher, &QFutureWatcher<bool>::finished, this, [this]() {
            m_result = m_futureWatcher->result();
            qDebug() << "控制结果： " << m_result;
            emit resultChanged();
        });
    }
    bool result() const { return m_result; }
        
    Q_INVOKABLE void test() {
        auto fw = m_cc->controlcpp();
        m_futureWatcher->setFuture(fw);
    }

    Q_SIGNAL void resultChanged();
private:
    QFutureWatcher<bool>* m_futureWatcher;
    ControllerControl *m_cc;
    bool m_result = false;
};