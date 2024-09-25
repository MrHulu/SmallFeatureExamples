#include "ControllerControl.h"
#include <QJSEngine>
#include <QtConcurrent>


// 构造函数
ControllerControl::ControllerControl(QObject *parent, QVariant data, size_t size, quint16 timeout)
    : QObject(parent)
    , m_data(data)
    , m_size(size)
    , m_timeout(timeout)
    , m_futureWatcher(new QFutureWatcher<bool>(this))
{
    // 连接 futureWatcher 的完成信号到 lambda 函数
    // 当异步操作完成时，发出 controlResultReady 信号
    connect(m_futureWatcher, &QFutureWatcher<bool>::finished, this, [this]() {
        emit controlResultReady(m_futureWatcher->result());
    });
}

// 析构函数
ControllerControl::~ControllerControl()
{
    // 释放 futureWatcher 资源
    delete m_futureWatcher;
}

// 发送控制信号并返回一个 Promise
QJSValue ControllerControl::control()
{
    // 获取当前的 JavaScript 引擎
    QJSEngine *engine = qjsEngine(this);
    if (!engine) {
        qWarning() << "无法获取 JavaScript 引擎";
        return QJSValue();
    }

    PromiseResolver *resolver = new PromiseResolver(this);
    QJSValue promiseResolverJS = engine->newQObject(resolver);

    QJSValue promise = engine->evaluate(R"(
        (function(resolver) {
            return new Promise((resolve, reject) => {
                resolver.resolved.connect(resolve);
                resolver.rejected.connect(reject);
            });
        })
    )").call(QJSValueList() << promiseResolverJS);

    QtConcurrent::run([this, resolver]() {

        if(m_status == Pending) {
            resolver->reject(QJSValue("控制操作正在进行中 "));
            resolver->deleteLater();
            return;
        }
        setStatus(Pending);
        emit requestControl(m_data, m_size);
        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        bool result = false;
        connect(&timer, &QTimer::timeout, &loop, [&]() {
            result = false;
            setStatus(Failure);
            loop.quit();
        }); // 超时处理
        connect(this, &ControllerControl::controlResultReady, &loop, [&](bool success) {
            result = success;
            setStatus(result ? Success : Failure);
            loop.quit();
        });
        timer.start(m_timeout);
        loop.exec();

        QTimer::singleShot(0, this, [resolver, result]() {
            if (result) {
                resolver->resolve(QJSValue(true));
            } else {
                resolver->reject(QJSValue("控制操作失败 "));
            }
            resolver->deleteLater();
        });
    });

    return promise;
}

// 接收控制结果的槽函数
void ControllerControl::receiveControlResult(const QVariant &result)
{
    // 异步处理控制结果
    QFuture<bool> future = QtConcurrent::run(this, &ControllerControl::processControlResult, result);
    m_futureWatcher->setFuture(future);
}

// 处理控制结果的私有方法
bool ControllerControl::processControlResult(const QVariant &result)
{
    return result.toBool();
}

