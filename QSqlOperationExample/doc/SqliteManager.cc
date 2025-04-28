//
// Created by Jayce on 2023/11/16.
//

#include "SqliteManager.h"

#include <QFileInfo>

#include "FileUtil.h"

namespace Util
{

SqliteManager::SqliteManager()
{
    QString filename = getDatabaseFilePath();
    QFileInfo file(filename);
    if (!file.isFile()) {
        // TODO: 如果找不到文件应该从后台下载进来
        throw SQLException("The parameter.db file cannot be found!");
    }

    // 建立和Sqlite数据库的连接,并加载sqlite驱动
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    // 设置数据库文件名称，在这里是通过文件的绝对路径找到该文件
    m_db.setDatabaseName(filename);
    m_query = QSqlQuery(m_db);
    m_query.setForwardOnly(true);
}

SqliteManager::~SqliteManager()
{
    closeDB();
}

void SqliteManager::openDB()
{
    if (m_db.isOpen()) { return; }
    if (m_db.open()) {
        qDebug() << "open parameter.db file success!  ";
    } else {
        throw SQLException("open parameter.db file error!", m_db.lastError());
    }
}

void SqliteManager::closeDB()
{
    m_db.close();
}

QString SqliteManager::getDatabaseFilePath()
{
    return FileUtil::splicePath({FileUtil::getDocumentsPath(), "/MOZA Pit House/parameter.db"});
}

int SqliteManager::runCommand(const QString &sql, const QVariantList &args)
{
//    qDebug() << "sql:" << sql << args;
    m_query.prepare(sql);
    for (auto &item: args) { m_query.addBindValue(item); }
    if (!m_query.exec()) { return -1; }
    return m_query.numRowsAffected();
}

QVariantList SqliteManager::getOneColumnList(const QString &sql, const QVariantList &args)
{
    QVariantList result;
    if (runCommand(sql, args) < 0) {
        throw SQLException("run sql error!", m_db.lastError());
    }

    if (m_query.record().isEmpty()) { return {}; }
    while (m_query.next()) {
        QSqlRecord cur = m_query.record();
        result.append(cur.value(0));
    }

    m_query.finish();
    return result;
}

QVariant SqliteManager::getOneItem(const QString &sql, const QVariantList &args)
{
    if (runCommand(sql, args) < 0) {
        throw SQLException("run sql error!", m_db.lastError());
    }

    // 判空以及跳过header
    if (m_query.record().isEmpty() || !m_query.next()) { return {}; }

    QVariant result = m_query.record().value(0);
    m_query.finish();
    return result;
}

}
