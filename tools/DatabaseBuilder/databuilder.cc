#include "databuilder.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDir>

#include <easylogging++.h>



DataBuilder::DataBuilder(const std::string& sqlFilename, 
                         const std::string& sqliteFilename) :
                         m_lastQuerySuccessful(false), m_connections(0)
{
    m_databaseFile = QString(sqliteFilename.c_str());
    m_sqlFile = QString(sqlFilename.c_str());
    
    if (!QFile(m_databaseFile).exists()) {
        LOG(ERROR) << "Database not found [" << m_databaseFile.toStdString() << "] !";
    }
    
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
    m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDatabase.setDatabaseName(m_databaseFile);
}

DataBuilder::~DataBuilder()
{   
    if (m_sqlDatabase.isOpen()) m_sqlDatabase.close();
}

bool DataBuilder::build()
{
    QFile file(m_sqlFile);
    DCHECK(file.open(QIODevice::ReadOnly)) 
            << "Unable to open SQL file [" << m_sqlFile << "]";
    
    QTextStream in(&file);
    
    DLOG(INFO) << "Reading SQL file...";
    QStringList sqlFromFile;
    while(!in.atEnd()) {
        sqlFromFile.append(in.readLine());
    }
    
    file.close();
    DLOG(INFO) << "Building database... [" << m_databaseFile << "]";
    foreach (QString sql, sqlFromFile) {
        sql = sql.trimmed();
        if (sql.isEmpty() || sql.startsWith("--")) {
            continue;
        }
        LOG_EVERY_N(100, DEBUG) << "Current iteration [" 
                                << ELPP_COUNTER_POS << "]; SQL: " << sql;
        query(sql);
        if (!m_lastQuerySuccessful) {
            return false;
        }
    }
    return m_lastQuerySuccessful;
}

const QList<QSqlRecord>& DataBuilder::query(const QString& query, 
                                            const QVariantMap& arguments) {
    m_lastQueryResult.clear();
    QString connectionName = "Builder_connection" + QString::number(++m_connections);
    VLOG(9) << "Making connection using [" << connectionName << "]";
    if (!m_sqlDatabase.open()) {
        DLOG(ERROR) << "Could not open database [" << connectionName << "]";
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
