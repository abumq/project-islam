#include "db/database_manager.hh"
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include "log/logging.hh"

namespace db {

DatabaseManager::DatabaseManager(const QString& uniqueId) : 
        m_uniqueId(uniqueId), m_lastQuerySuccessful(false), m_connections(0) {
    DATABASE_LOG(DEBUG) << "Initializing DatabaseManager [" << m_uniqueId << "]";
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
    m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDatabase.setDatabaseName(db::kDefaultDatabaseName);
}

DatabaseManager::~DatabaseManager(void) {
    DATABASE_LOG(DEBUG) << "Destroying DatabaseManager [" << m_uniqueId << "]";
    if (m_sqlDatabase.isOpen()) m_sqlDatabase.close();
}

const db::QueryResult& DatabaseManager::query(const QString& query, const QVariantMap& arguments) {
    m_lastQueryResult.clear();
    m_lastQuery = query;
    QString connectionName = m_uniqueId + "_connection" + QString::number(++m_connections);
    DATABASE_LOG(DEBUG) << "Making connection using [" << connectionName << "]";
    if (!m_sqlDatabase.open()) {
        DATABASE_LOG(ERROR) << "Could not open database [" << connectionName << "]";
        return m_lastQueryResult;
    }
    QSqlQuery sqlQuery(m_sqlDatabase);
    sqlQuery.prepare(query);
    foreach (QString key, arguments.keys()) {
        DATABASE_LOG(DEBUG) << "Binding [" << key << " -> " << arguments.value(key).toString() << "]";
        sqlQuery.bindValue(key, arguments.value(key));
    }
    
    if (!sqlQuery.exec()) {
        m_lastQuerySuccessful = false;
        DATABASE_LOG(ERROR) << "Could not execute query [" << sqlQuery.executedQuery() << "] : " << sqlQuery.lastError().text();
        return m_lastQueryResult;
    } else {
        m_lastQuerySuccessful = true;
        DATABASE_LOG(INFO) << "Successfully executed [" << sqlQuery.executedQuery() << "]";
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

const db::QueryResult& DatabaseManager::lastQueryResult(void) const {
    return m_lastQueryResult;
}

const QString& DatabaseManager::lastQuery(void) const {
    return m_lastQuery;
}

bool DatabaseManager::lastQuerySuccessful(void) const {
    return m_lastQuerySuccessful;
}

} // namespace db
