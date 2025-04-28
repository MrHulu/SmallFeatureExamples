#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

#include "Database.h"
#include "Exception.h"

namespace SqliteUtils {

SqliteDatabase SqliteDatabase::create(const QString& connectionName, const QString& dbFilePath) {
    SqliteDatabase db(connectionName);
    
    // 设置数据库路径并打开
    db.m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.m_db.setDatabaseName(dbFilePath);
    
    if (!db.m_db.open()) {
        throw SQLException("Can't open database!", db.m_db.lastError());
    }
    
    return db;
}

bool SqliteDatabase::isOpen() const {
    return m_db.isOpen();
}

void SqliteDatabase::close() {
    if (isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase(m_db.connectionName());
}

bool SqliteDatabase::beginTransaction() {
    return m_db.transaction();
}

bool SqliteDatabase::commitTransaction() {
    return m_db.commit();
}

bool SqliteDatabase::rollbackTransaction() {
    return m_db.rollback();
}

int SqliteDatabase::execute(const QString& sql, const QVariantList& args) {
    if (!executeQuery(sql, args)) {
        return -1;
    }
    return m_query.numRowsAffected();
}

QVariant SqliteDatabase::queryValue(const QString& sql, const QVariantList& args) {
    if (!executeQuery(sql, args) || !m_query.next()) {
        return QVariant();
    }
    return m_query.value(0);
}

QVariantMap SqliteDatabase::queryRow(const QString& sql, const QVariantList& args) {
    QVariantMap result;
    
    if (!executeQuery(sql, args) || !m_query.next()) {
        return result;
    }
    
    QSqlRecord record = m_query.record();
    for (int i = 0; i < record.count(); i++) {
        result.insert(record.fieldName(i), record.value(i));
    }
    
    return result;
}

QList<QVariantMap> SqliteDatabase::queryRows(const QString& sql, const QVariantList& args) {
    QList<QVariantMap> result;
    
    if (!executeQuery(sql, args)) {
        return result;
    }
    
    while (m_query.next()) {
        QVariantMap row;
        QSqlRecord record = m_query.record();
        
        for (int i = 0; i < record.count(); i++) {
            row.insert(record.fieldName(i), record.value(i));
        }
        
        result.append(row);
    }
    
    return result;
}

QVariantList SqliteDatabase::queryColumn(const QString& sql, const QVariantList& args) {
    QVariantList result;
    
    if (!executeQuery(sql, args)) {
        return result;
    }
    
    while (m_query.next()) {
        result.append(m_query.value(0));
    }
    
    return result;
}

qint64 SqliteDatabase::lastInsertId() const {
    return m_query.lastInsertId().toLongLong();
}

int SqliteDatabase::affectedRows() const {
    return m_query.numRowsAffected();
}

QSqlError SqliteDatabase::lastError() const {
    return m_query.lastError();
}

QSqlDatabase& SqliteDatabase::database() {
    return m_db;
}

SqliteDatabase::SqliteDatabase(const QString& connectionName) {}

bool SqliteDatabase::executeQuery(const QString& sql, const QVariantList& args) {
    m_query = QSqlQuery(m_db);
    m_query.prepare(sql);
    
    // 绑定参数
    for (int i = 0; i < args.size(); i++) {
        m_query.bindValue(i, args.at(i));
    }
    
    if (!m_query.exec()) {
        return false;
    }
    
    return true;
}

} // namespace SqliteUtils 