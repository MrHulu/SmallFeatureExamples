#pragma once

#include <QObject>

class ScopedConnection {
public:
    // 构造函数，初始化为空
    ScopedConnection() = default;

    // 赋值重载，用于存储新连接，并且自动断开旧连接
    ScopedConnection& operator=(const QMetaObject::Connection& connection) {
        // 如果已经存在连接，则断开
        if (m_connection) {
            QObject::disconnect(m_connection);
        }
        // 保存新的连接
        m_connection = connection;
        return *this;
    }

    // 析构函数，在对象销毁时自动断开连接
    ~ScopedConnection() {
        if (m_connection) {
            QObject::disconnect(m_connection);
        }
    }

private:
    QMetaObject::Connection m_connection = QMetaObject::Connection();
};
