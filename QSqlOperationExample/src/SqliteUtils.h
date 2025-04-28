#pragma once

// 包含所有SqliteUtils模块的头文件
#include "Exception.h"
#include "Database.h"
#include "QueryHelper.h"
#include "Repository.h"

/**
 * @brief SqliteUtils命名空间包含使用SQLite数据库的工具类
 * 
 * 主要组件:
 * - SQLException: 处理SQL错误的异常类
 * - SqliteDatabase: 数据库连接和操作类
 * - QueryHelper: 简化对象查询的助手类
 * - Repository<T>: 通用的数据访问仓储基类
 * 
 * 使用示例:
 * @code
 * #include <SqliteUtils.h>
 * 
 * // 创建数据库连接
 * auto db = SqliteUtils::SqliteDatabase::create("myConnection", "database.db");
 * 
 * // 执行查询
 * auto results = db.queryRows("SELECT * FROM users WHERE age > ?", {18});
 * 
 * // 使用Repository模式
 * UserRepository repo(db);
 * auto users = repo.findWhere("age BETWEEN ? AND ?", {18, 30});
 * @endcode
 */ 