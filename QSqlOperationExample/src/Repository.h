#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtCore/QUuid>

#include "QueryHelper.h"

namespace SqliteUtils {

// 通用仓储基类，简化CRUD操作
template<typename T>
class Repository {
public:
    explicit Repository(SqliteDatabase& db) : m_db(db), m_helper(new QueryHelper(db)) {}
    virtual ~Repository() = default;

    // 表名（子类必须实现）
    virtual QString tableName() const = 0;
    
    // 主键列名（子类必须实现）
    virtual QString primaryKeyColumn() const = 0;
    
    // 查找所有记录
    QList<T> findAll() {
        QString sql = QString("SELECT * FROM %1").arg(tableName());
        return m_helper->queryObjects<T>(sql);
    }
    
    // 按主键查找
    std::optional<T> findById(const QVariant& id) {
        QString sql = QString("SELECT * FROM %1 WHERE %2 = ?")
                     .arg(tableName())
                     .arg(primaryKeyColumn());
        return m_helper->queryObject<T>(sql, {id});
    }
    
    // 按条件查找
    QList<T> findWhere(const QString& whereClause, const QVariantList& args = {}) {
        QString sql = QString("SELECT * FROM %1 WHERE %2")
                     .arg(tableName())
                     .arg(whereClause);
        return m_helper->queryObjects<T>(sql, args);
    }
    
    // 检查对象是否存在
    bool exists(const QVariant& id) {
        QString sql = QString("SELECT COUNT(*) FROM %1 WHERE %2 = ?")
                     .arg(tableName())
                     .arg(primaryKeyColumn());
        return m_db.queryValue(sql, {id}).toInt() > 0;
    }
    
    // 保存对象（插入或更新）
    // 需要子类实现toMap和extractId方法
    bool save(const T& entity) {
        QVariant id = extractId(entity);
        QVariantMap data = toMap(entity);
        
        if(id.isValid() && !id.isNull() && exists(id)) {
            return update(id, data);
        } else {
            return insert(data);
        }
    }
    
    // 通过ID删除对象
    bool deleteById(const QVariant& id) {
        QString sql = QString("DELETE FROM %1 WHERE %2 = ?")
                     .arg(tableName())
                     .arg(primaryKeyColumn());
        
        return m_db.execute(sql, {id}) > 0;
    }

protected:
    // 将对象转换为数据映射（子类实现）
    virtual QVariantMap toMap(const T& entity) const = 0;
    
    // 从对象提取主键值（子类实现）
    virtual QVariant extractId(const T& entity) const = 0;
    
    // 基础插入操作
    bool insert(const QVariantMap& data) {
        if (data.isEmpty()) {
            return false;
        }
        
        QStringList columns;
        QStringList placeholders;
        QVariantList values;
        
        for (auto it = data.begin(); it != data.end(); ++it) {
            // 跳过主键字段，让数据库自动生成
            if (it.key() != primaryKeyColumn()) {
                columns.append(it.key());
                placeholders.append("?");
                values.append(it.value());
            }
        }
        
        if (columns.isEmpty()) {
            return false;
        }
        
        QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                     .arg(tableName())
                     .arg(columns.join(", "))
                     .arg(placeholders.join(", "));
        
        return m_db.execute(sql, values) > 0;
    }
    
    // 基础更新操作
    bool update(const QVariant& id, const QVariantMap& data) {
        if (data.isEmpty()) {
            return false;
        }
        
        QStringList setStatements;
        QVariantList values;
        
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it.key() != primaryKeyColumn()) {
                setStatements.append(it.key() + " = ?");
                values.append(it.value());
            }
        }
        
        if (setStatements.isEmpty()) {
            return false;
        }
        
        values.append(id);
        
        QString sql = QString("UPDATE %1 SET %2 WHERE %3 = ?")
                     .arg(tableName())
                     .arg(setStatements.join(", "))
                     .arg(primaryKeyColumn());
        
        return m_db.execute(sql, values) > 0;
    }
    
    SqliteDatabase& m_db;
    QueryHelper* m_helper = nullptr;
};

} // namespace SqliteUtils 