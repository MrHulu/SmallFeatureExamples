#include "ControllerControl.h"
#include <QJSEngine>
#include <QtConcurrent>


// 构造函数
ControllerControl::ControllerControl(QObject *parent, QVariant data, quint16 timeout)
    : QObject(parent)
    , m_data(data)
    , m_timeout(timeout)
    , m_futureWatcher(new QFutureWatcher<bool>(this))
    , m_resolver(new PromiseResolver(this))
{

}

// 析构函数
ControllerControl::~ControllerControl()
{
    // 释放 futureWatcher 资源
    delete m_futureWatcher;
    delete m_resolver;
}

// 发送控制信号并返回一个 Promise
QFuture<bool> ControllerControl::controlcpp() 
{
    return QtConcurrent::run([this]() {
        
        if(m_status == Pending) {
            m_resolver->reject(QJSValue("控制操作正在进行中 "));
            m_resolver->deleteLater();
            return false;
        }
        setStatus(Pending);
        emit requestControl(m_data);

        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        bool result = false;
        connect(&timer, &QTimer::timeout, &loop, [&]() {
            result = false;
            loop.quit();
        }); // 超时处理
        connect(this, &ControllerControl::controlResultReady, &loop, [&](bool success) {
            result = success;
            loop.quit();
        });
        timer.start(m_timeout);
        loop.exec();

        QTimer::singleShot(0, this, [this, result]() {
            if (m_resolver) {
                if (result) {
                    m_resolver->resolve(QJSValue(true));
                } else {
                    m_resolver->reject(QJSValue("控制操作失败 "));
                }
            }
            setStatus(result ? Success : Failure);
        });

        return result;
    });
}

QJSValue ControllerControl::control()
{
    QJSEngine *engine = qjsEngine(this);
    if(status() == Pending) {
        if(engine)
            return engine->evaluate("Promise.revject('Control operation is in progress')");
        else
            return QJSValue();
    }
    if (!engine) {
        qWarning() << "无法获取 JavaScript 引擎";
        setStatus(Failure);
        return QJSValue();
    }

    if (m_resolver) {
        delete m_resolver;
        m_resolver = nullptr;
    }
    m_resolver = new PromiseResolver(this);
    QJSValue promiseResolverJS = engine->newQObject(m_resolver);

    QJSValue promise = engine->evaluate(R"(
        (function(resolver) {
            return new Promise((resolve, reject) => {
                resolver.resolved.connect(resolve);
                resolver.rejected.connect(reject);
            });
        })
    )").call(QJSValueList() << promiseResolverJS);

    QFuture<bool> future = controlcpp();
    m_futureWatcher->setFuture(future);

    return promise;
}

// 接收控制结果的槽函数
void ControllerControl::receiveControlResult(const QVariant &result)
{
    auto success = processControlResult(result);
    emit controlResultReady(success);
}

// 处理控制结果的私有方法
bool ControllerControl::processControlResult(const QVariant &result)
{
    return result.toBool();
}

