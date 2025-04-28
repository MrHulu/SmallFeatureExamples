//
// Created by Jayce on 2023/11/16.
//

#pragma once

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "BaseSingleton.h"
#include "SQLException.h"

namespace Util
{

template<typename T>
concept bean = requires(const QVariantMap &item) {
    { T(item) } -> std::convertible_to<T>;
};

class SqliteManager: public BaseSingleton<SqliteManager>
{
    // 单例模式
    friend class BaseSingleton<SqliteManager>;
    SqliteManager();
    ~SqliteManager();
public:
    SqliteManager(const SqliteManager &) = delete;
    SqliteManager &operator=(const SqliteManager &) = delete;

    /**
     * <h2>打开数据库</h2>
     * @throw Util::SQLException 打开数据库错误
     */
    void openDB();
    void closeDB();

    /**
     * <h2>获取参数库db文件的绝对路径</h2>
     * @return 绝对路径字符串
     */
    static QString getDatabaseFilePath();

    /**
     * <h2>执行一次查询操作，并返回实体类列表</h2>
     * <p>使用示例：runQuery\<ParameterViewPO\>("SELECT * FROM ParameterView");</p>
     * <p>他会查询ParameterView表的所有数据，然后把数据保存到ParameterViewPO实体类中并返回其数据列表</p>
     * @tparam T 需要查询的实体类，必须继承自AbstractPO
     * @param sql 查询语句
     * @param args 查询参数
     * @return 查询得到的实体类列表
     * @throw Util::SQLException 执行数据库语句异常
     */
    template<bean T>
    QList<T> queryList(const QString &sql, const QVariantList& args = {});

    /**
     * <h2>查询一行数据，并返回实体类列表</h2>
     * @tparam T 需要查询的实体类，必须继承自AbstractPO
     * @param sql 查询语句
     * @param args 查询参数
     * @return 查询得到的实体类对象
     * @throw Util::SQLException 执行数据库语句异常
     */
    template<bean T>
    std::optional<T> queryBean(const QString &sql, const QVariantList& args = {});

    /**
     * <h2>一般用于执行一次增删改操作</h2>
     * @param sql 操作语句
     * @param args 操作参数
     * @return 返回受结果SQL语句影响的行数，出错时返回-1
     */
    int runCommand(const QString &sql, const QVariantList& args = {});

    /**
     * <h2>查询一列数据</h2>
     * @param sql 查询语句
     * @param args 查询参数
     * @return 查询得到的数据列表，如果有多列则只返回第一列
     * @throw Util::SQLException 执行数据库语句异常
     */
    QVariantList getOneColumnList(const QString &sql, const QVariantList& args = {});

    /**
     * <h2>查询一项数据</h2>
     * @param sql 查询语句
     * @param args 查询参数
     * @return 查询得到的数据，如果有多个数据则只返回第一行第一列
     * @throw Util::SQLException 执行数据库语句异常
     */
    QVariant getOneItem(const QString &sql, const QVariantList& args = {});

private:
    QSqlDatabase m_db;
    QSqlQuery m_query;
};

template<bean T>
QList<T> SqliteManager::queryList(const QString &sql, const QVariantList &args)
{
    QList<T> result;
    if (runCommand(sql, args) < 0) {
        throw SQLException("run sql error!", m_db.lastError());
    }

    while (m_query.next()) {
        QSqlRecord cur = m_query.record();
        QVariantMap item;
        for (int i = 0; i < cur.count(); ++i) {
            item.insert(cur.fieldName(i), cur.value(i));
//            qDebug() << cur.fieldName(i) << "===>" << cur.value(i);
        }
        result.append(T(item));
    }

    m_query.finish();
    return result;
}

template<bean T>
std::optional<T> SqliteManager::queryBean(const QString &sql, const QVariantList &args)
{
    if (runCommand(sql, args) < 0) {
        throw SQLException("run sql error!", m_db.lastError());
    }

    // 判空以及跳过header
    if (m_query.record().isEmpty() || !m_query.next()) { return {}; }

    QSqlRecord cur = m_query.record();
    QVariantMap item;
    for (int i = 0; i < cur.count(); ++i) {
        item.insert(cur.fieldName(i), cur.value(i));
//        qDebug() << cur.fieldName(i) << "===>" << cur.value(i);
    }

    m_query.finish();
    return T(item);
}

}
