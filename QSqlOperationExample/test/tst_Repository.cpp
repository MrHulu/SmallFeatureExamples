#include <QtTest>
#include <QTemporaryFile>
#include "Database.h"
#include "Repository.h"
#include <optional>

// 定义一个简单的测试模型
class User {
public:
    User() = default;
    explicit User(const QVariantMap& data) 
        : id(data["id"].toString()), 
          username(data["username"].toString()), 
          email(data["email"].toString()) {}
    
    std::optional<QString> id;
    std::optional<QString> username;
    std::optional<QString> email;
};

// 用户仓储类
class UserRepository : public SqliteUtils::Repository<User> {
public:
    explicit UserRepository(SqliteUtils::SqliteDatabase& db) : Repository<User>(db) {}
    
    QString tableName() const override { return "users"; }
    QString primaryKeyColumn() const override { return "id"; }
    
    QVariantMap toMap(const User& user) const override {
        QVariantMap map;
        if(user.id.has_value())
            map["id"] = user.id.value();
        if(user.username.has_value())
            map["username"] = user.username.value();
        if(user.email.has_value())
            map["email"] = user.email.value();
        return map;
    }
    
    QVariant extractId(const User& user) const override {
        return user.id.value_or(QString());
    }
};

class TestRepository : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    
    // 测试CRUD操作
    void testCrudOperations();
    
    // 测试查询操作
    void testQueryOperations();
    
    // 清理测试
    void cleanupTestCase();

private:
    QString dbPath;
    SqliteUtils::SqliteDatabase* db = nullptr;
    UserRepository* repo = nullptr;
};

void TestRepository::initTestCase()
{
    // 创建临时数据库
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(false);
    if (tempFile.open()) {
        dbPath = tempFile.fileName();
        tempFile.close();
    } else {
        dbPath = ":memory:";
    }
    
    // 初始化数据库和仓储
    db = new SqliteUtils::SqliteDatabase(SqliteUtils::SqliteDatabase::create("repoTest", dbPath));
    
    // 创建测试表
    db->execute("CREATE TABLE users("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "username TEXT UNIQUE, "
                "email TEXT)");
    
    repo = new UserRepository(*db);
}

void TestRepository::testCrudOperations()
{
    // 创建测试用户
    User user1;
    user1.username = "zhangsan";
    user1.email = "zhangsan@example.com";
    
    // 测试保存（插入）
    bool result = repo->save(user1);
    QVERIFY(result);
    
    // 测试查找
    auto users = repo->findAll();
    QCOMPARE(users.size(), 1);
    QCOMPARE(users[0].username, QString("zhangsan"));
    
    // 测试通过ID查找
    QVERIFY(users[0].id.has_value());
    auto foundOpt = repo->findById(users[0].id.value());
    QVERIFY(foundOpt.has_value());
    User found = foundOpt.value();
    QCOMPARE(found.username, QString("zhangsan"));
    QCOMPARE(found.email, QString("zhangsan@example.com"));
    
    // 测试更新
    User userToUpdate = found;
    userToUpdate.email = "updated@example.com";
    result = repo->save(userToUpdate);
    QVERIFY(result);
    
    // 验证更新成功
    foundOpt = repo->findById(userToUpdate.id.value_or(QString()));
    QVERIFY(foundOpt.has_value());
    found = foundOpt.value();
    QCOMPARE(found.email, QString("updated@example.com"));
    
    // 测试删除
    result = repo->deleteById(found.id.value_or(QString()));
    QVERIFY(result);
    
    // 验证删除成功
    users = repo->findAll();
    QVERIFY(users.isEmpty());
}

void TestRepository::testQueryOperations()
{
    // 创建多个测试用户
    User user1, user2, user3;
    
    user1.username = "user1";
    user1.email = "user1@example.com";
    
    user2.username = "user2";
    user2.email = "user2@example.com";
    
    user3.username = "admin";
    user3.email = "admin@example.com";
    
    // 保存用户
    repo->save(user1);
    repo->save(user2);
    repo->save(user3);
    
    // 测试findAll
    auto allUsers = repo->findAll();
    QCOMPARE(allUsers.size(), 3);
    
    // 测试条件查询
    auto users = repo->findWhere("username LIKE ?", {"%user%"});
    QCOMPARE(users.size(), 2);
    
    // 测试exists
    QVERIFY(repo->exists(users[0].id.value_or(QString())));
    QVERIFY(!repo->exists(999)); // 不存在的ID
}

void TestRepository::cleanupTestCase()
{
    // 清理资源
    delete repo;
    repo = nullptr;
    
    if (db) {
        db->close();
        delete db;
        db = nullptr;
    }
    
    // 删除临时数据库文件
    if (dbPath != ":memory:") {
        QFile::remove(dbPath);
    }
}

QTEST_MAIN(TestRepository)
#include "tst_Repository.moc" 