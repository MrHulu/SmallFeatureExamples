#pragma once

#include <optional>
#include <QList>

namespace Util { class SqliteManager; }

namespace controller::parameter
{

class CommandOperator;
class CommunicationParameter;
class ServiceParameter;
class EepromParameter;

class SQLiteDAO: public Util::BaseSingleton<SQLiteDAO>
{
    friend class Util::BaseSingleton<SQLiteDAO>;
    SQLiteDAO();
public:
    SQLiteDAO(const SQLiteDAO &) = delete;
    SQLiteDAO &operator=(const SQLiteDAO &) = delete;

    std::optional<CommandOperator> getCommandOperatorByName(const QString &name);
    std::optional<CommunicationParameter> getCommunicationParameterByName(const QString &name);
    std::optional<ServiceParameter> getServiceParameterByName(const QString &name, const QString &categoryPath);
    std::optional<EepromParameter> getEepromParameter(int table_id, int address);
    std::optional<EepromParameter> getEepromParameter(int table_id, const QString &name);
    QList<EepromParameter> findEepromParameters(const QString &categoryPath);

    std::optional<CommandOperator> findReadCommandOperator(const QString &communicationParameterName);
    std::optional<CommandOperator> findWriteCommandOperator(const QString &communicationParameterName);
    QList<CommunicationParameter> findCommunicationParametersOrderByRequestIndex(const QString &commandOperatorName);
    QList<CommunicationParameter> findCommunicationParametersOrderByResponseIndex(const QString &commandOperatorName);
    int findRequestBeginIndex(const QString &commandOperatorName, const QString &communicationParameterName);
    int findResponseBeginIndex(const QString &commandOperatorName, const QString &communicationParameterName);

    QStringList findTotalServiceParameterCategoryPathList();
    QStringList findTotalEepromParameterCategoryPathList();
    QStringList getServiceParameterNameList(const QString &categoryPath);

private:
    std::optional<CommandOperator> findReadOrWriteCommandOperator(const QString &communicationParameterName, bool isRead);

private:
    Util::SqliteManager *manager_;
};

}