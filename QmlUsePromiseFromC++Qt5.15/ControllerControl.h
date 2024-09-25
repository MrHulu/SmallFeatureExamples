#pragma once

#include <QObject>
#include <QByteArray>
#include <QFuture>
#include <QFutureWatcher>
#include <QJSValue>
class ControllerControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    enum Status {
        Idle = 0,
        Pending,
        Success,
        Failure
    };
    Q_ENUM(Status);
    Status status() const { return m_status; }
    Q_SIGNAL void statusChanged();
private: 
    void setStatus(Status status) {
        if (m_status == status)
            return;
        m_status = status;
        emit statusChanged();
    }
    Status m_status = Idle;

public:
    explicit ControllerControl(QObject *parent = nullptr, QVariant data = QByteArray(), size_t size = 0, quint16 timeout = 5000);
    ~ControllerControl();

    Q_INVOKABLE QJSValue control();
    Q_SIGNAL void requestControl(QVariant data = QByteArray(), int size = 0);

private:
    Q_SIGNAL void controlResultReady(bool success);

public slots:
    void receiveControlResult(const QVariant &result);

private:
    QFutureWatcher<bool> *m_futureWatcher;
    QVariant m_data = QByteArray();
    size_t m_size = 0;
    quint16 m_timeout = 5000;
    bool processControlResult(const QVariant &result);
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
