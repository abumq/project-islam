#ifndef _LOGGER
#   define _LOGGER "data"
#endif
#ifndef _PERFORMANCE_LOGGER
#   define _PERFORMANCE_LOGGER _LOGGER
#endif

#include "core/data/database_manager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

#include "core/logging/logging.h"
#include "core/utils/filesystem.h"

namespace data {

DatabaseManager::DatabaseManager(const QString& uniqueId, const QString& dbFilename) : 
        m_uniqueId(uniqueId), m_lastQuerySuccessful(false), m_connections(0) {
    DLOG(INFO) << "Initializing DatabaseManager [" << m_uniqueId << "]";
    const QString kDefaultDatabasePath = filesystem::buildPath(QStringList() << SettingsLoader::getInstance().defaultHomeDir() << "data");
    const QString kDefaultDatabaseName = kDefaultDatabasePath + dbFilename;
    
    if (!QFile(kDefaultDatabaseName).exists()) {
        LOG(ERROR) << "Database not found [" << kDefaultDatabaseName << "] ! Please make sure you have correct home path. Current ["
                    << SettingsLoader::getInstance().defaultHomeDir() << "]";
    }
    
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
    m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDatabase.setHostName(SettingsLoader::getInstance().get(SettingsLoader::kSettingKeyDatabaseHost, QVariant(QString(""))).toString());
    m_sqlDatabase.setPort(SettingsLoader::getInstance().get(SettingsLoader::kSettingKeyDatabasePort, QVariant(-1)).toInt());
    m_sqlDatabase.setDatabaseName(kDefaultDatabaseName);
}

DatabaseManager::~DatabaseManager(void) {
    LOG(DEBUG) << "Destroying DatabaseManager [" << m_uniqueId << "]";
    if (m_sqlDatabase.isOpen()) m_sqlDatabase.close();
}

const data::QueryResult& DatabaseManager::query(const QString& query, const QVariantMap& arguments) {
    m_lastQueryResult.clear();
    m_lastQuery = query;
    QString connectionName = m_uniqueId + "_connection" + QString::number(++m_connections);
    VLOG(9) << "Making connection using [" << connectionName << "]";
    if (!m_sqlDatabase.open()) {
        DLOG(ERROR) << "Could not open database using connection [" << connectionName << "]";
        return m_lastQueryResult;
    }
    QSqlQuery sqlQuery(m_sqlDatabase);
    sqlQuery.prepare(query);
    foreach (QString key, arguments.keys()) {
        DLOG(DEBUG) << "Binding [" << key << " -> " << arguments.value(key).toString() << "]";
        sqlQuery.bindValue(key, arguments.value(key));
    }
    
    if (!sqlQuery.exec()) {
        m_lastQuerySuccessful = false;
        DLOG(ERROR) << "Could not execute query [" << sqlQuery.executedQuery() << "] : " << sqlQuery.lastError().text();
        return m_lastQueryResult;
    } else {
        m_lastQuerySuccessful = true;
        VLOG(9) << "Successfully executed [" << sqlQuery.executedQuery() << "]";
    }
    while(sqlQuery.next()) {
        m_lastQueryResult << sqlQuery.record();
    }
    m_sqlDatabase.close();
    QSqlDatabase::removeDatabase(connectionName);
    return m_lastQueryResult;
}

void DatabaseManager::reset(void) {
    m_connections = 0;
}

const data::QueryResult& DatabaseManager::lastQueryResult(void) const {
    return m_lastQueryResult;
}

const QString& DatabaseManager::lastQuery(void) const {
    return m_lastQuery;
}

bool DatabaseManager::lastQuerySuccessful(void) const {
    return m_lastQuerySuccessful;
}

} // namespace data
