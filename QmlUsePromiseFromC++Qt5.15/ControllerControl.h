#pragma once

#include <QObject>
#include <QByteArray>
#include <QFuture>
#include <QFutureWatcher>
#include <QJSValue>

class ControllerControl : public QObject
{
    Q_OBJECT
public:
    explicit ControllerControl(QObject *parent = nullptr);
    ~ControllerControl();

    Q_INVOKABLE QJSValue sendControlSignal();
    Q_SIGNAL void controlSignalSent();

private:
    Q_SIGNAL void controlResultReady(bool success);

public slots:
    void receiveControlResult(const QByteArray &result);

private:
    QFutureWatcher<bool> *m_futureWatcher;
    bool processControlResult(const QByteArray &result);
};




class PromiseResolver : public QObject
{
    Q_OBJECT
public:
    explicit PromiseResolver(QObject *parent = nullptr) : QObject(parent) {}
    Q_INVOKABLE void resolve(const QJSValue &value) { emit resolved(value); }
    Q_INVOKABLE void reject(const QJSValue &reason) { emit rejected(reason); }
signals:
    void resolved(const QJSValue &value);
    void rejected(const QJSValue &reason);
};

