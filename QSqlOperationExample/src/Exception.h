#pragma once

#include <QtSql/QSqlError>

namespace SqliteUtils {

// 简化的异常类，用于表示SQL错误
class SQLException {
public:
    SQLException(const QString& message, const QSqlError& error = QSqlError())
        : message_(message), error_(error) {}

    QString message() const { return message_; }
    QSqlError error() const { return error_; }

private:
    QString message_;
    QSqlError error_;
};

} // namespace SqliteUtils 