#pragma once

#include <QObject>
#include <QQmlPropertyMap>

class Test : public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlPropertyMap* ts READ ts NOTIFY tsChanged)
    Q_PROPERTY(bool isChinese READ isChinese WRITE setIsChinese NOTIFY isChineseChanged)
public:
    explicit Test(QObject *parent = nullptr);

    QQmlPropertyMap* ts() const { return m_ts; }
    bool isChinese() const { return m_isChinese; }
    void setIsChinese(bool isChinese);

signals:
    void tsChanged();
    void isChineseChanged();

private:
    QQmlPropertyMap* m_ts;
    bool m_isChinese = false;
};

