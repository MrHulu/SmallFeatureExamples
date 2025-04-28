#include "SQLiteDAO.h"

#include <QJsonArray>
#include <SqliteManager.h>

#include "CommandOperator.h"
#include "CommunicationParameter.h"
#include "ServiceParameter.h"
#include "EepromParameter.h"

namespace controller::parameter
{

SQLiteDAO::SQLiteDAO()
{
    manager_ = Util::SqliteManager::instance();
    manager_->openDB();
}

std::optional<CommandOperator> SQLiteDAO::getCommandOperatorByName(const QString &name)
{
    QString sql = "SELECT * FROM CommandOperator WHERE name = ? ";
    try {
        return manager_->queryBean<CommandOperator>(sql, {name});
    } catch (const Util::SQLException &e) {
        qWarning() << "获取CommandOperator时发生了数据库异常:" << e.what() << e.error();
        return {};
    } catch (const std::exception &e) {
        qWarning() << "获取CommandOperator时发生了其他异常" << e.what();
        return {};
    }
}

std::optional<CommunicationParameter> SQLiteDAO::getCommunicationParameterByName(const QString &name)
{
    QString sql = "SELECT * FROM CommunicationParameter WHERE name = ? ";
    try {
        return manager_->queryBean<CommunicationParameter>(sql, {name});
    } catch (const Util::SQLException &e) {
        qWarning() << "获取CommunicationParameter时发生了数据库异常:" << e.what() << e.error();
        return {};
    } catch (const std::exception &e) {
        qWarning() << "获取CommunicationParameter时发生了其他异常" << e.what();
        return {};
    }
}

std::optional<ServiceParameter> SQLiteDAO::getServiceParameterByName(const QString &name, const QString &categoryPath)
{
    QString sql = "SELECT * FROM ServiceParameter WHERE name = ? AND category_path = ? ";
    try {
        return manager_->queryBean<ServiceParameter>(sql, {name, categoryPath});
    } catch (const Util::SQLException &e) {
        qWarning() << "获取ServiceParameter时发生了数据库异常:" << e.what() << e.error();
        return {};
    } catch (const std::exception &e) {
        qWarning() << "获取ServiceParameter时发生了其他异常" << e.what();
        return {};
    }
}

std::optional<CommandOperator> SQLiteDAO::findReadCommandOperator(const QString &communicationParameterName)
{
    return findReadOrWriteCommandOperator(communicationParameterName, true);
}

std::optional<CommandOperator> SQLiteDAO::findWriteCommandOperator(const QString &communicationParameterName)
{
    return findReadOrWriteCommandOperator(communicationParameterName, false);
}

std::optional<CommandOperator> SQLiteDAO::findReadOrWriteCommandOperator(const QString &communicationParameterName, bool isRead)
{
    QString sql = "SELECT command_name FROM ParameterIndex WHERE parameter_name = ? AND read_or_write = ? ";
    QString readOrWrite = isRead ? "read" : "write ";
    QString commandName;
    try {
        auto item = manager_->getOneItem(sql, {communicationParameterName, readOrWrite});
        if (item.isNull()) { return {}; }
        commandName = item.toString();
    } catch (const Util::SQLException &e) {
        qWarning() << "获取command_name时发生了数据库异常:" << e.what() << e.error();
        return {};
    }
    return getCommandOperatorByName(commandName);
}

QStringList SQLiteDAO::findTotalServiceParameterCategoryPathList()
{
    QString sql = "SELECT DISTINCT category_path FROM ServiceParameter ";
    QVariantList res;
    try {
        res = manager_->getOneColumnList(sql);
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::findTotalServiceParameterCategoryPathList:" << e.what() << e.error();
        return {};
    }
    QStringList ans;
    for (const auto &item : qAsConst(res)) {
        ans.push_back(item.toString());
    }
    return ans;
}

QStringList SQLiteDAO::getServiceParameterNameList(const QString &categoryPath)
{
    QString sql = "SELECT name FROM ServiceParameter WHERE category_path = ? ";
    QVariantList res;
    try {
        res = manager_->getOneColumnList(sql, {categoryPath});
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::getServiceParameterNameList:" << e.what() << e.error();
        return {};
    }
    QStringList ans;
    for (const auto &item : qAsConst(res)) {
        ans.push_back(item.toString());
    }
    return ans;
}

QList<CommunicationParameter> SQLiteDAO::findCommunicationParametersOrderByRequestIndex(const QString &commandOperatorName)
{
    QString sql = "SELECT parameter_name FROM ParameterIndex WHERE command_name = ? ORDER BY request_begin_bit_index ";
    QVariantList res;
    try {
        res = manager_->getOneColumnList(sql, {commandOperatorName});
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::findTotalCommunicationParameter:" << e.what() << e.error();
        return {};
    }
    QList<CommunicationParameter> ans;
    for (const auto &item : qAsConst(res)) {
        auto param = getCommunicationParameterByName(item.toString());
        if (param.has_value()) { ans.push_back(param.value()); }
    }
    return ans;
}

QList<CommunicationParameter> SQLiteDAO::findCommunicationParametersOrderByResponseIndex(const QString &commandOperatorName)
{
    QString sql = "SELECT parameter_name FROM ParameterIndex WHERE command_name = ? ORDER BY response_begin_bit_index ";
    QVariantList res;
    try {
        res = manager_->getOneColumnList(sql, {commandOperatorName});
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::findTotalCommunicationParameter:" << e.what() << e.error();
        return {};
    }
    QList<CommunicationParameter> ans;
    for (const auto &item : qAsConst(res)) {
        auto param = getCommunicationParameterByName(item.toString());
        if (param.has_value()) { ans.push_back(param.value()); }
    }
    return ans;
}

int SQLiteDAO::findRequestBeginIndex(const QString &commandOperatorName, const QString &communicationParameterName)
{
    QString sql = "SELECT request_begin_bit_index FROM ParameterIndex WHERE command_name = ? AND parameter_name = ? ";
    try {
        auto item = manager_->getOneItem(sql, {commandOperatorName, communicationParameterName});
        if (item.isNull()) { return -1; }
        return item.toInt();
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::getCommunicationParameterName:" << e.what() << e.error();
        return -1;
    }
}

int SQLiteDAO::findResponseBeginIndex(const QString &commandOperatorName, const QString &communicationParameterName)
{
    QString sql = "SELECT response_begin_bit_index FROM ParameterIndex WHERE command_name = ? AND parameter_name = ? ";
    try {
        auto item = manager_->getOneItem(sql, {commandOperatorName, communicationParameterName});
        if (item.isNull()) { return -1; }
        return item.toInt();
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::getCommunicationParameterName:" << e.what() << e.error();
        return -1;
    }
}

std::optional<EepromParameter> SQLiteDAO::getEepromParameter(int table_id, int address)
{
    QString sql = "SELECT * FROM EepromParameter WHERE table_id = ? AND address = ? ";
    try {
        return manager_->queryBean<EepromParameter>(sql, {table_id, address});
    } catch (const Util::SQLException &e) {
        qWarning() << "获取EepromParameter时发生了数据库异常:" << e.what() << e.error();
        return {};
    } catch (const std::exception &e) {
        qWarning() << "获取EepromParameter时发生了其他异常" << e.what();
        return {};
    }
}

std::optional<EepromParameter> SQLiteDAO::getEepromParameter(int table_id, const QString &name)
{
    QString sql = "SELECT * FROM EepromParameter WHERE table_id = ? AND name = ? ";
    try {
        return manager_->queryBean<EepromParameter>(sql, {table_id, name});
    } catch (const Util::SQLException &e) {
        qWarning() << "获取EepromParameter时发生了数据库异常:" << e.what() << e.error();
        return {};
    } catch (const std::exception &e) {
        qWarning() << "获取EepromParameter时发生了其他异常" << e.what();
        return {};
    }
}

QStringList SQLiteDAO::findTotalEepromParameterCategoryPathList()
{
    QString sql = "SELECT DISTINCT category_path FROM EepromParameter ";
    QVariantList res;
    try {
        res = manager_->getOneColumnList(sql);
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::findTotalEepromParameterCategoryPathList:" << e.what() << e.error();
        return {};
    }
    QStringList ans;
    for (const auto &item : qAsConst(res)) {
        ans.push_back(item.toString());
    }
    return ans;
}

QList<EepromParameter> SQLiteDAO::findEepromParameters(const QString &categoryPath)
{
    QString sql = "SELECT * FROM EepromParameter WHERE category_path = ? ";
    try {
        return manager_->queryList<EepromParameter>(sql, {categoryPath});
    } catch (const Util::SQLException &e) {
        qWarning() << "SQLException in SQLiteDAO::findEepromParameters:" << e.what() << e.error();
        return {};
    }
}

}
