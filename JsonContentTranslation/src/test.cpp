#include "test.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

Test::Test(QObject *parent) : QObject(parent) {
    m_ts = new QQmlPropertyMap(this);
    auto path = QString(":/Info.json");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to Info.json file:" << file.errorString();
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();
    QJsonObject hulu = root["HULU"].toObject();
    for (auto it = hulu.begin(); it != hulu.end(); it++) {
        m_ts->insert(it.key(), it.value().toString());
    }
}

void Test::setIsChinese(bool isChinese)
{
    m_isChinese = isChinese;
    emit isChineseChanged();
}
