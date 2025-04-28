#pragma once

#include <optional>
#include <QtCore/QList>
#include <QtCore/QVariantMap>

#include "Database.h"

namespace SqliteUtils {

// 对象关系映射接口
template<typename T>
concept BeanObject = requires(const QVariantMap& map) {
    { T(map) } -> std::convertible_to<T>;
};

// 数据对象查询助手类
class QueryHelper {
public:
    explicit QueryHelper(SqliteDatabase& db) : m_db(db) {}
    
    // 查询单个对象
    template<BeanObject T>
    std::optional<T> queryObject(const QString& sql, const QVariantList& args = {}) {
        auto row = m_db.queryRow(sql, args);
        if (row.isEmpty()) {
            return std::nullopt;
        }
        return T(row);
    }
    
    // 查询对象列表
    template<BeanObject T>
    QList<T> queryObjects(const QString& sql, const QVariantList& args = {}) {
        QList<T> result;
        auto rows = m_db.queryRows(sql, args);
        for (const auto& row : rows) {
            result.append(T(row));
        }
        return result;
    }
    
private:
    SqliteDatabase& m_db;
};

} // namespace SqliteUtils 