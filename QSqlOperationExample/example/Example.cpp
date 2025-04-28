#include <iostream>
#include <QDebug>
#include "User.h"

// 初始化数据库表
void initDatabase(SqliteUtils::SqliteDatabase& db) {
    // 创建用户表
    db.execute("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT NOT NULL,"
               "email TEXT,"
               "age INTEGER"
               ")");
}

// 演示基本操作
void demoBasicOperations(SqliteUtils::SqliteDatabase& db) {
    qDebug() << "===== 基本数据库操作演示 ===== ";
    
    // 插入数据
    db.execute("INSERT INTO users (name, email, age) VALUES (?, ?, ?)",
              {"李四", "lisi@example.com", 30});
    
    // 查询单个值
    QVariant count = db.queryValue("SELECT COUNT(*) FROM users");
    qDebug() << "用户总数: " << count.toInt() ;
    
    // 查询单行
    QVariantMap user = db.queryRow("SELECT * FROM users WHERE name = ?", {"李四"});
    if (!user.isEmpty()) {
        qDebug() << "查找到用户: " << user["name"].toString()
                 << ", 邮箱: " << user["email"].toString() ;
    }
    
    // 查询多行
    QList<QVariantMap> users = db.queryRows("SELECT * FROM users WHERE age > ?", {20});
    qDebug() << "查找到 " << users.size() << " 个年龄大于20的用户 ";
}

// 演示Repository模式
void demoRepositoryPattern(SqliteUtils::SqliteDatabase& db) {
    qDebug() << "\n===== Repository模式演示 ===== ";
    UserRepository userRepo(db);
    
    // 创建新用户
    User newUser;
    newUser.name = "张三 ";
    newUser.email = "zhangsan@example.com ";
    newUser.age = 25;
    
    // 保存用户
    if (userRepo.save(newUser)) {
        qDebug() << "保存用户成功 ";
    }
    
    // 查询所有用户
    QList<User> allUsers = userRepo.findAll();
    qDebug() << "总共 " << allUsers.size() << " 个用户 ";
    
    for (const auto& user : allUsers) {
        qDebug() << "ID: " << user.id.value_or(QString())
                 << ", 姓名: " << user.name.value_or(QString())
                 << ", 年龄: " << user.age.value_or(0) ;
    }
    
    // 使用自定义查询
    QList<User> youngUsers = userRepo.findByAgeRange(20, 30);
    qDebug() << "\n年龄在20-30之间的用户: " << youngUsers.size() << " 人 ";
    
    for (const auto& user : youngUsers) {
        qDebug() << user.name.value_or(QString()) << ", " << user.age.value_or(0) << "岁 ";
    }
}

// 演示事务操作
void demoTransactions(SqliteUtils::SqliteDatabase& db) {
    qDebug() << "\n===== 事务操作演示 ===== ";
    UserRepository userRepo(db);
    
    db.beginTransaction();
    
    try {
        // 批量操作
        User user1, user2;
        
        user1.name = "王五 ";
        user1.email = "wangwu@example.com ";
        user1.age = 40;
        
        user2.name = "赵六 ";
        user2.email = "zhaoliu@example.com ";
        user2.age = 35;
        
        userRepo.save(user1);
        userRepo.save(user2);
        
        qDebug() << "保存了两个用户 ";
        
        // 提交事务
        db.commitTransaction();
        qDebug() << "事务已提交 ";
    } catch (const std::exception& e) {
        db.rollbackTransaction();
        std::cerr << "事务已回滚: " << e.what() ;
    }
}

int main() {
    try {
        // 创建内存数据库用于示例
        auto db = SqliteUtils::SqliteDatabase::create("exampleConnection", ":memory:");
        
        // 初始化数据库
        initDatabase(db);
        
        // 运行演示
        demoBasicOperations(db);
        demoRepositoryPattern(db);
        demoTransactions(db);
        
        // 关闭数据库
        db.close();
        
        qDebug() << "\n示例运行完成! ";
    } catch (const SqliteUtils::SQLException& e) {
        std::cerr << "数据库错误: " << e.message().toStdString();
        if (e.error().isValid()) {
            std::cerr << " (" << e.error().text().toStdString() << ") ";
        }
        std::cerr ;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() ;
        return 1;
    }
    
    return 0;
} 