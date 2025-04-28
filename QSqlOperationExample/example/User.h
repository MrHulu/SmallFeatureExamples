#pragma once

#include "../src/SqliteUtils.h"

#include <QtCore/QVariantMap>
#include <QtCore/QString>

class User {
public:
    User() = default;
    
    // 从数据库记录构造
    explicit User(const QVariantMap& data) {
        id = data["id"].toString();
        name = data["name"].toString();
        email = data["email"].toString();
        age = data["age"].toInt();
    }
    
    std::optional<QString> id;
    std::optional<QString> name;
    std::optional<QString> email;
    std::optional<int >age;
};


// 用户仓储
class UserRepository : public SqliteUtils::Repository<User> {
public:
    explicit UserRepository(SqliteUtils::SqliteDatabase& db) 
        : Repository<User>(db) {}
    
    QString tableName() const override { return "users"; }
    QString primaryKeyColumn() const override { return "id"; }
    
    QVariantMap toMap(const User& user) const override {
        QVariantMap data;
        if(user.id.has_value())
            data["id"] = user.id.value();
        if(user.name.has_value())
            data["name"] = user.name.value();
        if(user.email.has_value())
            data["email"] = user.email.value();
        if(user.age.has_value())
            data["age"] = user.age.value();
        return data;
    }
    
    QVariant extractId(const User& user) const override {
        if(user.id.has_value())
            return user.id.value();
        return QVariant();
    }
    
    // 自定义查询示例
    QList<User> findByAgeRange(int minAge, int maxAge) {
        return findWhere("age BETWEEN ? AND ?", {minAge, maxAge});
    }
};