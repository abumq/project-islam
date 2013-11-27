#include "core/data/database_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include "core/logging.h"

namespace data {

DatabaseManager::DatabaseManager(const QString& uniqueId) : 
        m_uniqueId(uniqueId), m_lastQuerySuccessful(false), m_connections(0) {
    DDATA_LOG(INFO) << "Initializing DatabaseManager [" << m_uniqueId << "]";
    
    const QString kDefaultDatabasePath = SettingsLoader().defaultHomeDir() + "data" + QDir::separator();
    const QString kDefaultDatabaseName = kDefaultDatabasePath + "project-islam.db";
    
    if (!QFile(kDefaultDatabaseName).exists()) {
        LOG(ERROR) << "Database not found! Please make sure you have correct home path. Current ["
                    << SettingsLoader().defaultHomeDir() << "]";
    }
    
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
    m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDatabase.setDatabaseName(kDefaultDatabaseName);
}

DatabaseManager::~DatabaseManager(void) {
    DATA_LOG(DEBUG) << "Destroying DatabaseManager [" << m_uniqueId << "]";
    if (m_sqlDatabase.isOpen()) m_sqlDatabase.close();
}

const data::QueryResult& DatabaseManager::query(const QString& query, const QVariantMap& arguments) {
    m_lastQueryResult.clear();
    m_lastQuery = query;
    QString connectionName = m_uniqueId + "_connection" + QString::number(++m_connections);
    VDATA_LOG(9) << "Making connection using [" << connectionName << "]";
    if (!m_sqlDatabase.open()) {
        DDATA_LOG(ERROR) << "Could not open database [" << connectionName << "]";
        return m_lastQueryResult;
    }
    QSqlQuery sqlQuery(m_sqlDatabase);
    sqlQuery.prepare(query);
    foreach (QString key, arguments.keys()) {
        DATA_LOG(DEBUG) << "Binding [" << key << " -> " << arguments.value(key).toString() << "]";
        sqlQuery.bindValue(key, arguments.value(key));
    }
    
    if (!sqlQuery.exec()) {
        m_lastQuerySuccessful = false;
        DDATA_LOG(ERROR) << "Could not execute query [" << sqlQuery.executedQuery() << "] : " << sqlQuery.lastError().text();
        return m_lastQueryResult;
    } else {
        m_lastQuerySuccessful = true;
        VDATA_LOG(9) << "Successfully executed [" << sqlQuery.executedQuery() << "]";
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
