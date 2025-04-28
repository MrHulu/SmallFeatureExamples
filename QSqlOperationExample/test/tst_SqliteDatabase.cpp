#include <QtTest>
#include <QTemporaryFile>
#include "Database.h"

class TestSqliteDatabase : public QObject
{
    Q_OBJECT

private slots:
    // 数据库连接测试
    void testDatabaseConnection();
    
    // 事务管理测试
    void testTransactions();
    
    // 基本SQL操作测试
    void testBasicOperations();
    
    // 查询操作测试
    void testQueryOperations();
    
    // 错误处理测试
    void testErrorHandling();

private:
    // 创建临时测试数据库
    QString createTempDatabase();
};

void TestSqliteDatabase::testDatabaseConnection()
{
    // 创建临时数据库文件
    QString dbPath = createTempDatabase();
    
    // 测试创建数据库连接
    {
        auto db = SqliteUtils::SqliteDatabase::create("testConnection", dbPath);
        QVERIFY(db.isOpen());
        
        // 测试关闭连接
        db.close();
        QVERIFY(!db.isOpen());
    }
    
    // 测试异常情况 - 无效路径
    try {
        auto db = SqliteUtils::SqliteDatabase::create("invalidConnection", "/invalid/path/db.sqlite");
        QFAIL("应当抛出异常，但没有");
    } catch (const SqliteUtils::SQLException& e) {
        // 预期会抛出异常
        QVERIFY(true);
    }
}

void TestSqliteDatabase::testTransactions()
{
    QString dbPath = createTempDatabase();
    auto db = SqliteUtils::SqliteDatabase::create("transactionTest", dbPath);
    
    // 创建测试表
    db.execute("CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT)");
    
    // 测试事务提交
    {
        QVERIFY(db.beginTransaction());
        
        db.execute("INSERT INTO test_table (name) VALUES (?)", {"Test1"});
        db.execute("INSERT INTO test_table (name) VALUES (?)", {"Test2"});
        
        QVERIFY(db.commitTransaction());
        
        // 验证数据已提交
        auto count = db.queryValue("SELECT COUNT(*) FROM test_table").toInt();
        QCOMPARE(count, 2);
    }
    
    // 测试事务回滚
    {
        QVERIFY(db.beginTransaction());
        
        db.execute("INSERT INTO test_table (name) VALUES (?)", {"Test3"});
        db.execute("INSERT INTO test_table (name) VALUES (?)", {"Test4"});
        
        QVERIFY(db.rollbackTransaction());
        
        // 验证数据已回滚，记录数应该还是2
        auto count = db.queryValue("SELECT COUNT(*) FROM test_table").toInt();
        QCOMPARE(count, 2);
    }
    
    db.close();
}

void TestSqliteDatabase::testBasicOperations()
{
    QString dbPath = createTempDatabase();
    auto db = SqliteUtils::SqliteDatabase::create("basicTest", dbPath);
    
    // 创建测试表
    db.execute("CREATE TABLE test_crud (id INTEGER PRIMARY KEY, name TEXT, age INTEGER)");
    
    // 测试插入
    int rowsAffected = db.execute("INSERT INTO test_crud (name, age) VALUES (?, ?)", {"张三", 30});
    QCOMPARE(rowsAffected, 1);
    QVERIFY(db.lastInsertId() > 0);
    
    // 测试更新
    rowsAffected = db.execute("UPDATE test_crud SET age = ? WHERE name = ?", {31, "张三"});
    QCOMPARE(rowsAffected, 1);
    
    // 测试删除
    rowsAffected = db.execute("DELETE FROM test_crud WHERE name = ?", {"张三"});
    QCOMPARE(rowsAffected, 1);
    
    db.close();
}

void TestSqliteDatabase::testQueryOperations()
{
    QString dbPath = createTempDatabase();
    auto db = SqliteUtils::SqliteDatabase::create("queryTest", dbPath);
    
    // 创建并填充测试表
    db.execute("CREATE TABLE test_query (id INTEGER PRIMARY KEY, name TEXT, age INTEGER)");
    db.execute("INSERT INTO test_query (name, age) VALUES (?, ?)", {"张三", 30});
    db.execute("INSERT INTO test_query (name, age) VALUES (?, ?)", {"李四", 25});
    db.execute("INSERT INTO test_query (name, age) VALUES (?, ?)", {"王五", 40});
    
    // 测试queryValue
    QVariant value = db.queryValue("SELECT COUNT(*) FROM test_query");
    QCOMPARE(value.toInt(), 3);
    
    // 测试queryRow
    QVariantMap row = db.queryRow("SELECT * FROM test_query WHERE name = ?", {"李四"});
    QCOMPARE(row["name"].toString(), QString("李四"));
    QCOMPARE(row["age"].toInt(), 25);
    
    // 测试queryRows
    QList<QVariantMap> rows = db.queryRows("SELECT * FROM test_query ORDER BY age");
    QCOMPARE(rows.size(), 3);
    QCOMPARE(rows[0]["name"].toString(), QString("李四"));
    QCOMPARE(rows[1]["name"].toString(), QString("张三"));
    QCOMPARE(rows[2]["name"].toString(), QString("王五"));
    
    // 测试queryColumn
    QVariantList ages = db.queryColumn("SELECT age FROM test_query ORDER BY age");
    QCOMPARE(ages.size(), 3);
    QCOMPARE(ages[0].toInt(), 25);
    QCOMPARE(ages[1].toInt(), 30);
    QCOMPARE(ages[2].toInt(), 40);
    
    db.close();
}

void TestSqliteDatabase::testErrorHandling()
{
    QString dbPath = createTempDatabase();
    auto db = SqliteUtils::SqliteDatabase::create("errorTest", dbPath);
    
    // 创建测试表
    db.execute("CREATE TABLE test_error (id INTEGER PRIMARY KEY, name TEXT UNIQUE)");
    
    // 插入记录
    db.execute("INSERT INTO test_error (name) VALUES (?)", {"唯一名称"});
    
    // 测试约束失败 - 唯一性约束
    int result = db.execute("INSERT INTO test_error (name) VALUES (?)", {"唯一名称"});
    QCOMPARE(result, -1);
    QVERIFY(!db.lastError().text().isEmpty());
    
    // 测试SQL语法错误
    result = db.execute("SELEC * FROM test_error"); // 故意拼写错误
    QCOMPARE(result, -1);
    QVERIFY(!db.lastError().text().isEmpty());
    
    db.close();
}

QString TestSqliteDatabase::createTempDatabase()
{
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(false);
    if (tempFile.open()) {
        QString filePath = tempFile.fileName();
        tempFile.close();
        return filePath;
    }
    return ":memory:"; // 如果临时文件创建失败，使用内存数据库
}

QTEST_MAIN(TestSqliteDatabase)
#include "tst_SqliteDatabase.moc" 