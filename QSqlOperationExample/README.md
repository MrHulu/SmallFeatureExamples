# SQLite工具模块

这是一个基于Qt的SQLite数据库操作工具模块，提供简单易用的接口来进行数据库操作。

## 主要特性

- 简单的数据库连接管理
- 支持事务操作
- 简化SQL查询操作
- 支持对象关系映射(ORM)
- Repository模式简化CRUD操作

## 目录结构
├── README.md                   # 使用说明文档
├── src/                        # 实现文件目录
│   ├── Exception.h             # 异常类头文件
│   ├── QueryHelper.h           # 查询助手类头文件
│   ├── Repository.h            # 仓储基类头文件
│   ├── SqliteUtils.h           # 总头文件(包含其他所有头文件)
│   ├── Database.h              # 数据库类头文件
│   └── Database.cpp            # 数据库类实现
├── example/                    # 示例代码目录
│   └── Example.cpp             # 示例代码
├── test/                       # 测试代码目录
│   ├── tst_Repository.cpp      # 数据仓储模式测试
│   ├── tst_SqliteDatabase.cpp  # 核心数据库功能测试
│   └── tst_SqliteUtils.cpp     # 完整的集成测试
└── doc/                        # 文档和参考文件目录

```

## 依赖

- Qt 5.x 或更高版本
- QtSql 模块

## 使用方法

### 1. 包含头文件

```cpp
// 包含全部功能
#include <SqliteUtils.h>

// 也可以只包含需要的部分
#include <Database.h>
#include <Repository.h>
```

### 2. 数据库连接

```cpp
// 创建数据库连接
auto db = SqliteUtils::SqliteDatabase::create("myConnection", "path/to/database.db");

// 执行查询
auto rows = db.queryRows("SELECT * FROM users WHERE age > ?", {18});

// 关闭连接
db.close();
```

### 3. 创建数据模型

```cpp
// 定义一个用户数据模型
class User {
public:
    User() = default;
    
    // 从数据库记录构造（必须实现）
    explicit User(const QVariantMap& data) {
        id = data["id"].toInt();
        name = data["name"].toString();
        email = data["email"].toString();
        age = data["age"].toInt();
    }
    
    int id = 0;
    QString name;
    QString email;
    int age = 0;
};
```

### 4. 创建Repository

```cpp
// 用户仓储
class UserRepository : public SqliteUtils::Repository<User> {
public:
    explicit UserRepository(SqliteUtils::SqliteDatabase& db) 
        : Repository<User>(db) {}
    
    // 实现必要的方法
    QString tableName() const override { return "users "; }
    QString primaryKeyColumn() const override { return "id "; }
    
    // 将User对象转换为数据库字段映射
    QVariantMap toMap(const User& user) const override {
        QVariantMap data;
        data["id"] = user.id;
        data["name"] = user.name;
        data["email"] = user.email;
        data["age"] = user.age;
        return data;
    }
    
    // 提取用户ID
    QVariant extractId(const User& user) const override {
        return user.id;
    }
    
    // 添加额外的查询方法
    QList<User> findByAgeRange(int minAge, int maxAge) {
        return findWhere("age BETWEEN ? AND ?", {minAge, maxAge});
    }
};
```

### 5. 使用Repository

```cpp
// 创建数据库连接
auto db = SqliteUtils::SqliteDatabase::create("myConnection", "database.db");

// 创建用户仓储
UserRepository userRepo(db);

// 查询所有用户
auto allUsers = userRepo.findAll();

// 查询特定用户
auto user = userRepo.findById(1);
if (user.has_value()) {
    qDebug() << "找到用户: " << user->name.toStdString() ;
}

// 查询特定年龄范围的用户
auto youngUsers = userRepo.findByAgeRange(18, 30);

// 保存用户
User newUser;
newUser.name = "张三 ";
newUser.email = "zhangsan@example.com ";
newUser.age = 25;
userRepo.save(newUser);

// 删除用户
userRepo.deleteById(1);
```

## 事务支持

```cpp
auto db = SqliteUtils::SqliteDatabase::create("myConnection", "database.db");
UserRepository userRepo(db);

// 开始事务
db.beginTransaction();

try {
    // 执行多个操作
    User user1, user2;
    // 设置用户属性...
    
    userRepo.save(user1);
    userRepo.save(user2);
    
    // 提交事务
    db.commitTransaction();
} catch (...) {
    // 出错时回滚事务
    db.rollbackTransaction();
    throw;
}
```

## 设计原则

该模块基于SOLID原则设计：

1. **单一职责原则 (S)**：每个类都有明确的职责
   - `SqliteDatabase` 负责数据库连接和基本操作
   - `QueryHelper` 负责对象查询
   - `Repository` 负责数据访问层抽象

2. **开放封闭原则 (O)**：扩展开放，修改封闭
   - 可以通过继承 `Repository` 添加新的数据访问功能
   - 核心功能稳定，不需要修改

3. **里氏替换原则 (L)**：子类可以替换父类
   - 任何实现了必要接口的类都可以作为 `Repository` 使用

4. **接口隔离原则 (I)**：接口精简，职责单一
   - `BeanObject` concept 定义了最小的对象映射接口
   - 各组件接口清晰明确

5. **依赖倒置原则 (D)**：依赖抽象，不依赖实现
   - 高层模块通过抽象接口依赖底层模块

## 注意事项

- 请确保在适当的时候关闭数据库连接
- 使用参数化查询避免SQL注入问题
- 利用事务确保复杂操作的原子性