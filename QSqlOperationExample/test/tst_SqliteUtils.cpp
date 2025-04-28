#include <QtTest>
#include <QTemporaryFile>
#include "SqliteUtils.h"

// 定义一个测试用户类
class TestUser {
public:
    TestUser() = default;
    explicit TestUser(const QVariantMap& data) 
        : id(data["id"].toInt()), 
          name(data["name"].toString()), 
          email(data["email"].toString()), 
          age(data["age"].toInt()) {}
    
    int id = 0;
    QString name;
    QString email;
    int age = 0;
};

// 定义用户仓储类
class TestUserRepository : public SqliteUtils::Repository<TestUser> {
public:
    explicit TestUserRepository(SqliteUtils::SqliteDatabase& db) : Repository<TestUser>(db) {}
    
    QString tableName() const override { return "test_users"; }
    QString primaryKeyColumn() const override { return "id"; }
    
    QVariantMap toMap(const TestUser& user) const override {
        QVariantMap map;
        map["id"] = user.id;
        map["name"] = user.name;
        map["email"] = user.email;
        map["age"] = user.age;
        return map;
    }
    
    QVariant extractId(const TestUser& user) const override {
        return user.id;
    }
};

class TestSqliteUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    // 测试完整工作流程
    void testCompleteWorkflow();
    
    // 测试事务工作流程
    void testTransactionWorkflow();
    
    // 测试错误处理
    void testErrorHandling();

private:
    QString dbPath;
    SqliteUtils::SqliteDatabase* db = nullptr;
};

void TestSqliteUtils::initTestCase()
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
    
    // 初始化数据库
    db = new SqliteUtils::SqliteDatabase(SqliteUtils::SqliteDatabase::create("integrationTest", dbPath));
}

void TestSqliteUtils::cleanupTestCase()
{
    // 清理资源
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

void TestSqliteUtils::testCompleteWorkflow()
{
    // 创建表
    db->execute("CREATE TABLE test_users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name TEXT, "
                "email TEXT UNIQUE, "
                "age INTEGER)");
    
    // 初始化仓储
    TestUserRepository repo(*db);
    
    // 创建用户
    TestUser user1;
    user1.name = "张三";
    user1.email = "zhangsan@example.com";
    user1.age = 30;
    
    // 保存用户
    QVERIFY(repo.save(user1));
    
    // 查找用户
    auto users = repo.findAll();
    QCOMPARE(users.size(), 1);
    
    // 使用查询助手
    SqliteUtils::QueryHelper helper(*db);
    auto userOpt = helper.queryObject<TestUser>("SELECT * FROM test_users WHERE name = ?", {"张三"});
    QVERIFY(userOpt.has_value());
    QCOMPARE(userOpt->email, QString("zhangsan@example.com"));
    
    // 直接执行SQL查询
    QVariantMap row = db->queryRow("SELECT * FROM test_users WHERE age = ?", {30});
    QCOMPARE(row["name"].toString(), QString("张三"));
    
    // 更新用户
    TestUser userToUpdate = users[0];
    userToUpdate.age = 31;
    QVERIFY(repo.save(userToUpdate));
    
    // 验证更新
    auto updatedOpt = repo.findById(userToUpdate.id);
    QVERIFY(updatedOpt.has_value());
    QCOMPARE(updatedOpt->age, 31);
    
    // 删除用户
    QVERIFY(repo.deleteById(updatedOpt->id));
    
    // 验证删除
    users = repo.findAll();
    QVERIFY(users.isEmpty());
    
    // 清理表
    db->execute("DROP TABLE test_users");
}

void TestSqliteUtils::testTransactionWorkflow()
{
    // 创建表
    db->execute("CREATE TABLE test_tx_users ("
                "id INTEGER PRIMARY KEY, "
                "name TEXT, "
                "balance INTEGER)");
    
    // 开始事务
    db->beginTransaction();
    
    try {
        // 执行一系列操作
        db->execute("INSERT INTO test_tx_users (id, name, balance) VALUES (?, ?, ?)", {1, "用户A", 1000});
        db->execute("INSERT INTO test_tx_users (id, name, balance) VALUES (?, ?, ?)", {2, "用户B", 500});
        
        // 模拟转账
        db->execute("UPDATE test_tx_users SET balance = balance - ? WHERE id = ?", {100, 1});
        db->execute("UPDATE test_tx_users SET balance = balance + ? WHERE id = ?", {100, 2});
        
        // 提交事务
        db->commitTransaction();
        
        // 验证结果
        QVariantMap userA = db->queryRow("SELECT * FROM test_tx_users WHERE id = ?", {1});
        QVariantMap userB = db->queryRow("SELECT * FROM test_tx_users WHERE id = ?", {2});
        
        QCOMPARE(userA["balance"].toInt(), 900);
        QCOMPARE(userB["balance"].toInt(), 600);
    }
    catch (const SqliteUtils::SQLException& e) {
        // 发生错误，回滚事务
        db->rollbackTransaction();
        QFAIL("事务测试失败");
    }
    
    // 测试回滚
    db->beginTransaction();
    
    db->execute("UPDATE test_tx_users SET balance = balance - ? WHERE id = ?", {200, 1});
    
    // 故意回滚
    db->rollbackTransaction();
    
    // 验证回滚成功（余额应该保持不变）
    QVariantMap userA = db->queryRow("SELECT * FROM test_tx_users WHERE id = ?", {1});
    QCOMPARE(userA["balance"].toInt(), 900);
    
    // 清理表
    db->execute("DROP TABLE test_tx_users");
}

void TestSqliteUtils::testErrorHandling()
{
    // 创建表
    db->execute("CREATE TABLE test_error_handling ("
                "id INTEGER PRIMARY KEY, "
                "unique_code TEXT UNIQUE)");
    
    // 插入记录
    db->execute("INSERT INTO test_error_handling (id, unique_code) VALUES (?, ?)", {1, "CODE001"});
    
    // 测试唯一约束错误
    int result = db->execute("INSERT INTO test_error_handling (id, unique_code) VALUES (?, ?)", {2, "CODE001"});
    QCOMPARE(result, -1);
    QVERIFY(!db->lastError().text().isEmpty());
    
    // 测试SQL语法错误
    result = db->execute("SELEC * FROM test_error_handling");
    QCOMPARE(result, -1);
    
    // 测试异常处理
    bool exceptionCaught = false;
    try {
        // 创建一个新的不存在路径的数据库连接，应抛出异常
        SqliteUtils::SqliteDatabase invalidDb = 
            SqliteUtils::SqliteDatabase::create("invalidDb", "/invalid/path/to/db.sqlite");
    }
    catch (const SqliteUtils::SQLException& e) {
        exceptionCaught = true;
        QVERIFY(!e.message().isEmpty());
    }
    QVERIFY(exceptionCaught);
    
    // 清理表
    db->execute("DROP TABLE test_error_handling");
}

QTEST_MAIN(TestSqliteUtils)
#include "tst_SqliteUtils.moc" 