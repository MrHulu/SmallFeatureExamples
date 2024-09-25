#include "ControllerControl.h"
#include <QJSEngine>
#include <QtConcurrent>

// 构造函数
ControllerControl::ControllerControl(QObject *parent)
    : QObject(parent), m_futureWatcher(new QFutureWatcher<bool>(this))
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
QJSValue ControllerControl::sendControlSignal()
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
        emit controlSignalSent();

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
        timer.start(5000); // 5 秒超时
        loop.exec();

        QTimer::singleShot(0, this, [resolver, result]() {
            if (result) {
                resolver->resolve(QJSValue(true));
            } else {
                resolver->reject(QJSValue("控制操作失败"));
            }
            resolver->deleteLater();
        });
    });

    return promise;
}

// 接收控制结果的槽函数
void ControllerControl::receiveControlResult(const QByteArray &result)
{
    // 异步处理控制结果
    QFuture<bool> future = QtConcurrent::run(this, &ControllerControl::processControlResult, result);
    m_futureWatcher->setFuture(future);
}

// 处理控制结果的私有方法
bool ControllerControl::processControlResult(const QByteArray &result)
{
    // 这里应该根据实际需求处理结果
    return !result.isEmpty();
}