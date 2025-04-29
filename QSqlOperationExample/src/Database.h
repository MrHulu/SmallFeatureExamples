#pragma once

#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>

#include "Exception.h"

namespace SqliteUtils {

// 数据库管理类
class SqliteDatabase {
public:
    // 创建或打开数据库连接
    static SqliteDatabase create(const QString& connectionName, const QString& dbFilePath);
    ~SqliteDatabase();
    
    // 判断数据库是否已打开
    bool isOpen() const;
    
    // 关闭数据库连接
    void close();
    
    // 开始事务
    bool beginTransaction();
    
    // 提交事务
    bool commitTransaction();
    
    // 回滚事务
    bool rollbackTransaction();
    
    // 执行SQL命令（INSERT/UPDATE/DELETE等）
    int execute(const QString& sql, const QVariantList& args = {});
    
    // 查询返回单个值
    QVariant queryValue(const QString& sql, const QVariantList& args = {});
    
    // 查询返回单行数据
    QVariantMap queryRow(const QString& sql, const QVariantList& args = {});
    
    // 查询返回多行数据
    QList<QVariantMap> queryRows(const QString& sql, const QVariantList& args = {});
    
    // 查询返回单列数据
    QVariantList queryColumn(const QString& sql, const QVariantList& args = {});
    
    // 获取上一个插入的行ID
    qint64 lastInsertId() const;
    
    // 获取上一个操作影响的行数
    int affectedRows() const;
    
    // 获取最后一个错误
    QSqlError lastError() const;
    
    // 获取原始数据库对象（谨慎使用）
    QSqlDatabase& database();

private:
    SqliteDatabase(const QString& connectionName);
    
    QSqlDatabase m_db;
    QSqlQuery m_query;
    
    // 执行查询并处理错误
    bool executeQuery(const QString& sql, const QVariantList& args);
};

} // namespace SqliteUtils 